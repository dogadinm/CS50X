# TODO
text = input("Text: ")

letter = 0
word = 1
sentences = 0

for i in text:
    if i.isalpha():
        letter += 1
    elif i == " ":
        word += 1
    elif i == "." or i == "!" or i == "?":
        sentences += 1

index = 0.0588 * (letter/word * 100) - 0.296 * (sentences/word * 100) - 15.8

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade ", round(index))