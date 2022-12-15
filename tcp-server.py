#!/usr/bin/python3
'''
docstring
'''
import socket
import sys
import subprocess
import os
from http import HTTPStatus
import mimetypes

DEBUG = 1
DEFAULTFILE = '/index.html'
SEARCHPATH = '.'
CGIEXT = ['.cgi','.py']
HTTPVER = 'HTTP/1.1'
if sys.platform == "linux":
    PYTHON = 'python3'
elif sys.platform == "win32":
    PYTHON = 'py'

# PYTHON = 'python3'
DEF404SENDDATA = '''
HTTP/1.1 404 OK
Content-Type: text/html; charset=utf-8


<html>
<body>
<p>Not Found!</p>
</body>
</html>
'''
def parseHeaders(httpdata):
    '''
    Parsing HTTP Headers
    '''
    httpdata = httpdata.replace('\r', '')
    httpdata = httpdata.split('\n')
    method,url,version = httpdata[0].split(' ')
    if version != HTTPVER:
        print(version.encode())
        return HTTPStatus.HTTP_VERSION_NOT_SUPPORTED, b"\r\n\r\n n"
    os.environ["REQUEST_METHOD"] = method
    if '?' in url:
        path,getparams = url.split('?')
    else:
        path = url
        getparams = ''
    if path == '/exit':
        print("Server stopped")
        exit()
    os.environ['QUERY_STRING'] = getparams
    env = {}
    postdata = ''
    flag_post = 0
    # print("http:", httpdata)
    for i in httpdata:
        if flag_post:
            postdata += i+'\n'
        elif i == '':
            flag_post=1
        else:
            if ':' in i:
                env_key, env_value = i.split(":", maxsplit = 1) # User-Agent: Mozilla/5.0
                env_key = env_key.upper().replace("-","_") # User-Agent to USER_AGENT
                env_value = env_value[1:] # rem leading whitespace
                env[env_key] = env_value
                os.environ[env_key] = env_value
    # print(env)
    if os.path.isdir(SEARCHPATH+path):
        path += DEFAULTFILE
    # else: loc:5000/ 
        # path = path
    if DEBUG:
        print('REQUESTING FILE: ', path.encode())
    else:
        print(path,end=' ')
    ext = os.path.splitext(path)[-1].lower()
    if ext in CGIEXT:
        code, result_data = run_cgi(ext, SEARCHPATH+path, postdata)
    else:
        code, result_data = send_file(SEARCHPATH+path)
    return code, result_data
def run_cgi(ext, path, postdata):
    print("ext ",ext)
    my_env = os.environ.copy()
    if sys.platform == 'linux':
        sb = subprocess.Popen(path,stdout=subprocess.PIPE,stdin=subprocess.PIPE,
                              stderr=subprocess.PIPE, env=my_env)
        out, err = sb.communicate(input=bytes(postdata,encoding='utf8'))
    elif sys.platform == 'win32':
        if ext == '.py':
            print("CGI PYTHON")
            sb = subprocess.Popen(PYTHON+' '+path,stdout=subprocess.PIPE,stdin=subprocess.PIPE,
                                  stderr=subprocess.PIPE, env=my_env)
            # sb = subprocess.Popen(PYTHON+' '+path,stdout=subprocess.PIPE,
            # stdin=subprocess.PIPE,stderr=subprocess.PIPE, env=my_env)
            out, err = sb.communicate(input = bytes(postdata,encoding='utf8'))
            # out = out.encode('utf8')
        else:
            # path = path.replace('/','\\')
            print('path ',path)
            sb = subprocess.Popen('wsl'+' '+path, shell=True,stdout=subprocess.PIPE,stdin=subprocess.PIPE,
                                  stderr=subprocess.PIPE,env=my_env)
            # sb = subprocess.Popen('wsl'+' '+path, shell=True,stdout=subprocess.PIPE,stdin=subprocess.PIPE,stderr=subprocess.PIPE,env=my_env)
            out, err = sb.communicate(input=bytes(postdata,encoding='utf8'))

    print("CGI")
    if sb.returncode == 0:
        ret = out
        # print(ret)
        return HTTPStatus.OK, ret
    else:
        print("errorlog:", err)
        return HTTPStatus.INTERNAL_SERVER_ERROR, b' '+bytes(sb.returncode)

def send_file(path):
    mime = mimetypes.guess_type(path)[0]
    if not mime:
        mime = 'text/html'
    if DEBUG:
        print("SEND FILE "+mime)
    try:
        with(open(path,"rb")) as f:
            filedata = f.read()
    except FileNotFoundError:
        return HTTPStatus.NOT_FOUND, b' '
    return HTTPStatus.OK, b'Content-Type: '+bytes(mime,encoding='utf8')+b'; charset=UTF-8\r\n\r\n'+filedata

def main():
    # создаем TCP/IP сокет
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Привязываем сокет к порту
    server_address = ('localhost', 5000)
    print(f'Старт сервера на {server_address[0]} порт {server_address[1]}')
    sock.bind(server_address)

    # Слушаем входящие подключения
    sock.listen(1)

    while True:
        # ждем соединения
        print('waiting...',end='',flush=True)
        connection, client_address = sock.accept()
        try:
            print('\r            \r> ', client_address, end=' ')
            # Принимаем данные порциями и ретранслируем их
            while True:
                datamas = []
                data = b''
                while True:
                    drecv = connection.recv(2048)
                    data += drecv
                    if DEBUG:
                        print("d", drecv)
                    # break
                    if len(drecv) < 2048:
                        break
                if DEBUG:
                    print('Получено: ')
                    for i in datamas:
                        print(i)
                    print('END\n',flush=True)
                if data:
                    print('...',end=' ',flush=True)
                    code, senddata = parseHeaders(data.decode())
                    # data = data.upper()
                    print('>>>>',end=' ',flush=True)
                    # HTTP/1.1 200 OK
                    retdata =  HTTPVER+' '+str(code.value)+' '+code.phrase+'\n'
                    retdata = retdata.encode('utf8')
                    retdata += senddata
                    # print(retdata)
                    print(code.value,code.phrase,end=' ')
                    connection.sendall(retdata)
                    if DEBUG:
                        print(f'SEND: {len(retdata)}',flush=True)
                    # connection.sendall(defsenddata.encode('utf8'))
                    print()
                    break
                else:
                    print('Нет данных от:', client_address,flush=True)
                    break

        finally:
            # Очищаем соединение
            connection.close()
if __name__ == "__main__":
    while True:
        try:
            main()
        except Exception as e:
            print(e.with_traceback())
