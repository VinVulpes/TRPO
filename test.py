#!/usr/bin/python3
import os

print("Content-Type: text/html; charset=utf-8\r\n\r\n")
print()
print('''
<a href="./">Наверх</a>
<p>Загрузите файлы, чтобы посмотреть как выглядит запрос</p>
<form action="test.py" method="POST" enctype="multipart/form-data">
	<input type="file" name="myFile1">
    <br>
    <br>
	<input type="file" name="myFile2">
    <br>
    <br>
	<input type="submit" value="Загрузить">
</form>
''')
print(os.getenv('HTTP_USER_AGENT'))
print('<br>')
print(os.environ.get('SERVER_NAME'))
print('<br>')
print(os.environ.get('PATH_INFO'))
print('<br>')
print(os.environ.get('HTTP_CONTENT_LENGTH'))
print('<br>')
print(os.environ.get('CONTENT_TYPE'))
print('<br>')
contents = []
while True:
    try:
        line = input("")
    except EOFError:
        break
    contents.append(line)
print('content: ')
for i in contents:
    print(i+'<br>')
    # if()
