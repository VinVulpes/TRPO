all:
	g++ db.cpp HTTP.cpp working_with_db.cpp  -o db.cgi
	g++ HTTP.cpp cgi_handler.cpp -o test.cgi
db:
	g++ db.cpp HTTP.cpp working_with_db.cpp  -o db.cgi
test:
	g++ HTTP.cpp cgi_handler.cpp -o test.cgi