# TODO
import csv
import itertools
from sys import argv
import re
import cs50

# Check command line args
if len(argv) != 2:
    print("Usage: python import.py [csv_file]")
    exit(1)

# Open SQL database
open(f"students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# Create table
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# Open csv and begin parsing
with open(argv[1]) as dat_file:
    csv_reader = csv.DictReader(dat_file, delimiter=',')
    for row in csv_reader:
        full_name = row['name'].split()
        house = row['house']
        birth = row['birth']
        if len(full_name) == 3:     # If student has middle name, add that
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       full_name[0], full_name[1], full_name[2], house, birth)
        else:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       full_name[0], None, full_name[1], house, birth)