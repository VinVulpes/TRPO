.PHONY: all
all: db test files
db:
	g++ db.cpp HTTP.cpp working_with_db.cpp  -o db.cgi
test:
	g++ HTTP.cpp cgi_handler.cpp -o test.cgi
files:
	g++ db.cpp HTTP.cpp working_with_files.cpp -o uploadFile.cgi
clean:
	rm *.cgi