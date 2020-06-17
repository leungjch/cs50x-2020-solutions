import csv
import itertools
from sys import argv
import re
import cs50

# Check command line args
if len(argv) != 2:
    print("Usage: python roster.py [house_name]")
    exit(1)

# Open SQL database
db = cs50.SQL("sqlite:///students.db")
t = argv[1]

# Get list of students and their info
student_list = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", t)
for student in student_list:        # Print student info
    if student['middle'] == None:
        print(student['first'], student['last'], ', born', student['birth'])
    else:
        print(student['first'], student['middle'], student['last'], ', born', student['birth'])