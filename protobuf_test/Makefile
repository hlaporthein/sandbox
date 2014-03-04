all:
	protoc -I=. --cpp_out=. ./addressbook.proto
	g++ addressbook.pb.cc writer.cc -o writer -lprotobuf
	g++ addressbook.pb.cc reader.cc -o reader -lprotobuf

clean:
	rm -f *.exe *.pb* *.txt