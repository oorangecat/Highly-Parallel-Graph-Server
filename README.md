### C++ Server Component

To build the C++ part of the project, install dependencies or use nix shell:

	nix-shell


Then generate resources for protobuf (execute from project root folder):

    protoc -I=src/main/proto --cpp_out=src/main/cpp/proto src/main/proto/locations.proto


Then build the project with meson:

	meson setup builddir
	meson compile -C builddir

And finally run the project:

	./builddir/src/main/cpp/server <port> <format>

for example:

    ./builddir/src/main/cpp/server 