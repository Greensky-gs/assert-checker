OUTPUT=assert-checker

build: src/main.rs
	cargo build
buildrelease: src/main.rs
	cargo build --release

dev:	
	clear
	make build
	clear
	make run
release:
	clear
	make buildrelease
	clear
	./target/release/$(OUTPUT)

run:
	./target/debug/$(OUTPUT)
