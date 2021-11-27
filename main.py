# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import random
import sqlite3
import urllib.request
import re
from random import randint
from bs4 import BeautifulSoup


con = sqlite3.connect('main.db')
cursor = con.cursor()


# Press the green button in the gutter to run the script.
def get_content_url(url):
    u2 = urllib.request.urlopen(url)
    bytes = u2.read()
    return bytes.decode('utf8')


def get_book_index():
    global con, cursor
    sql_select = 'SELECT MAX(book_id) FROM books'
    for row in cursor.execute(sql_select):
        max_id = int(row[0]) if row[0] is not None else 0
    return max_id


def add_book(id_book, title, author, year, ISBN, rating, URL):
    global con, cursor
    sql_insert = f'INSERT INTO books VALUES({id_book},"{title}","{author}",{year},{ISBN},{rating},"{URL}")'
    try:
        cursor.execute(sql_insert)
    except sqlite3.IntegrityError:
        print("Book is already in db.")
        return False
    con.commit()
    print("Book added.")
    return True


def add_book_genres(page_content, book_index):
    global con, cursor
    genre_list = re.findall("<a class=\"actionLinkLite bookPageGenreLink\" href=\"/genres/(.*)</a>", page_content)
    for i, genre in enumerate(genre_list):
        index = genre.index('>')
        genre_list[i] = genre[index + 1:].replace("amp;", "")
    genre_dict = dict.fromkeys(genre_list)
    genre_dict.pop('Audiobook', None)
    genre_list = list(genre_dict)
    for genre in genre_list:
        sql = f'INSERT INTO book_genres VALUES({book_index},"{genre}")'
        cursor.execute(sql)
    con.commit()
    print("Genre added.")


def add_author_genres(author_name, author_link):
    global con, cursor
    sql = f'SELECT * FROM author_genres WHERE author="{author_name}"'
    if cursor.execute(sql).fetchone() is None:
        author_content = get_content_url(author_link)
        try:
            genres = re.search(
                "<div class=\"dataTitle\">Genre</div>\n\s*<div class=\"dataItem\">"
                "(.*)"
                "<div class=\"aboutAuthorInfo\">",
                author_content, re.DOTALL).group(1)
        except AttributeError:
            print("Author genres not found")
            return
        genres = genres[:genres.index("</div>")].replace("\n", "")
        soup = BeautifulSoup(genres, "html.parser")
        genre_list = []
        for row in soup.find_all('a'):
            genre = row.text.split(" & ")
            genre_list.extend(genre)
        for genre in genre_list:
            sql = f'INSERT INTO author_genres VALUES("{author_name}","{genre}")'
            cursor.execute(sql)
        con.commit()
        print("Author genres added.")
    else:
        print("Author is already in db.")


def get_book_info(book):
    book_content = get_content_url(f'https://www.goodreads.com/book/show/{book}')
    url = 'https://www.goodreads.com/book/show/' + book
    isbn = re.search("<meta content='(.*)' property='books:isbn'>", book_content).group(1)
    if isbn == 'null':
        print(f"{book} doesn't have isbn.")
        return None
    else:
        print(f"{book} has isbn: {isbn}")

    try:
        title = re.search("'og:type'>\n<meta content='(.*)' property='og:title'>", book_content, re.DOTALL).group(1)
    except AttributeError:
        title = re.search("'og:type'>\n<meta content=\"(.*)\" property='og:title'>", book_content, re.DOTALL).group(1)

    title = title.replace("amp;", "")
    print("Title is:", title)
    author_link = re.search("<meta content='(.*)' property='books:author'>", book_content).group(1)
    author_name = author_link[author_link.rfind('.') + 1:].replace("_", " ").replace("amp;", "")
    print("Author is:", author_name, "and can be found at", author_link)
    full_date = re.search("Published(.*)<div class=\"buttons\">", book_content, re.DOTALL).group(1)
    year = re.search(".*([1-3][0-9]{3})", full_date, re.DOTALL).group(1)
    print("Published on:", year)
    rating = re.search("<span itemprop=\"ratingValue\">\n(.*)\n</span>", book_content).group(1).replace(" ", "")
    print("Rating:", rating)
    index = get_book_index() + 1
    add_author_genres(author_name, author_link)
    if add_book(index, title, author_name, year, isbn, rating, url) is False:
        return None
    add_book_genres(book_content, index)
    return "ok"


def main():
    content = get_content_url(f'https://www.goodreads.com/list/show/8460.Best_Young_Adult_Realistic_Novels')

    book_list = re.findall("href=\"/book/show/(.*)\">", content)
    book_number = input(f"How many books to add(1-{len(book_list)})? ")
    i = 0
    while i < int(book_number):
        print("\nBook", i + 1)
        book_index = randint(0, len(book_list) - 1)
        if get_book_info(book_list[book_index]) is None:
            continue
        i += 1


main()
