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


def add_book(title, author, year, ISBN, rating, URL, image):
    global con, cursor
    sql_insert = f'INSERT INTO books VALUES("{title}","{author}",{year},{ISBN},{rating},"{URL}","{image}", NULL)'
    try:
        cursor.execute(sql_insert)
    except sqlite3.IntegrityError:
        print("Book is already in db.")
        return False
    else:
        con.commit()
        print("Book added.")
        return True


def add_book_genres(page_content, book_isbn):
    global con, cursor
    soup = BeautifulSoup(page_content, "html.parser")
    my_genres = soup.findAll("div", class_="elementList")
    if len(my_genres) == 0:
        return
    genre_dict = dict()
    for i, genre in enumerate(my_genres):
        if len(genre_dict) > 3:
            break
        all = genre.findAll('a')
        if len(all) == 0:
            continue
        book_genre = all[0].text
        if book_genre in genre_dict and genre_dict[book_genre] is not None or book_genre == '' or book_genre == 'Audiobook':
            continue
        if len(all) == 1:
            genre_dict[book_genre] = None
        else:
            book_subgenre = all[1].text
            genre_dict[book_genre] = book_subgenre

    print("genre_dict:", genre_dict)

    for genre, subgenre in genre_dict.items():
        sql_genre = f'INSERT INTO book_genres VALUES({book_isbn},"{genre}")'
        sql_subgenre = f'INSERT INTO book_genres VALUES({book_isbn},"{subgenre}")'
        sql_hierarchy = f'INSERT INTO genre_hierarchy VALUES("{genre}","{subgenre}")'
        try:
            cursor.execute(sql_genre)
        except sqlite3.IntegrityError:
            pass
        if subgenre is not None:
            try:
                cursor.execute(sql_subgenre)
            except sqlite3.IntegrityError:
                pass
            try:
                cursor.execute(sql_hierarchy)
            except sqlite3.IntegrityError:
                pass

    con.commit()
    print("Genres and subgenres added.")


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
        genre_list = list(dict.fromkeys(genre_list))
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
    try:
        full_date = re.search("Published(.*)<div class=\"buttons\">", book_content, re.DOTALL).group(1)
    except AttributeError:
        year = "NULL"
    else:
        year = re.search(".*([1-3][0-9]{3})", full_date, re.DOTALL).group(1)
        print("Published on:", year)
    rating = re.search("<span itemprop=\"ratingValue\">\n(.*)\n</span>", book_content).group(1).replace(" ", "")
    print("Rating:", rating)
    image_url = 'https://i.gr-assets.com/images/S/compressed.photo.goodreads.com/books/'
    image = image_url + re.search(f"<meta content='{image_url}(.*)' property='og:image'>", book_content).group(1)
    print("Image: ", image)
    add_author_genres(author_name, author_link)
    if add_book(title, author_name, year, isbn, rating, url, image) is False:
        return None
    add_book_genres(book_content, isbn)
    return "ok"


def main():
    input_url = input("Insert the url from Good Reads: ")
    content = get_content_url(input_url)

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
