text = input("Text: ")
l = 0   # Letters
s = 0   # Sentences
w = 0   # Words

for char in text:
    if char.isalnum():
        l += 1
    elif char.isspace():
        w += 1
    elif char == "!" or char == "." or char == "?":
        s += 1

w += 1  # account for extra word at the end

l_w = l / w * 100
s_w = s / w * 100

cl = 0.0588 * l_w - 0.296 * s_w - 15.8

if cl >= 16:
    print("Grade 16+")
elif cl < 1:
    print("Before Grade 1")
else:
    print("Grade ", round(cl))