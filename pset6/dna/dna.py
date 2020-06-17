import csv
import itertools
from sys import argv
import re

if len(argv) != 3:
    print("Usage: python dna.py [database_file] [sequence_file]")
    exit(1)

dat = []

# Load database
with open(argv[1]) as dat_file:
    csv_reader = csv.DictReader(dat_file, delimiter=',')
    for row in csv_reader:

        values = [int(row[x]) for x in list(row.keys())[1:]]    # Cast the values into int
        values.insert(0, row['name'])                           # Add the name back
        keys = list(row.keys())                                 # Get list of keys
        myDict = dict(zip(keys, values))                        # Make new dict with int-casted values
        dat.append(myDict)                                      # Append dict

# Cast dict items to string


# Get list of sequences to search
permutations = list(dat[0].keys())
permutations.remove("name")

# Make a copy of the dict with same keys
dat_matches = dat

# Load sequence
seq = []
with open(argv[2]) as seq_file:
    csv_reader = csv.reader(seq_file, delimiter=',')
    seq = next(csv_reader)

# Process sequence and count consecutive occurrences
counts = dict.fromkeys(permutations)
for perm in permutations:
    counter = 0
    consecutive = False
    candidate_counts = []
    matches = re.findall(f"((?:{perm})+)", seq[0])
    mymax = 0
    for item in matches:
        mymax = max(mymax, len(item)/len(perm))
    counts[perm] = mymax


# # Check for matches in database:
for entry in dat:
    # print(counts)
    compare = dict(entry)
    compare.pop("name")
    if compare == counts:
        print(entry['name'])
        # print('h')
        exit(0)
print("No match")
exit(1)