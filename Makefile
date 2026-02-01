CC = g++
PROGRAM = rezults


allp:
	$(CC) main.c lexer.c graphviz.c parser.c AST.c token.c transfer_asm.c asm.c function_asm.c -o $(PROGRAM)

trans:
	for file in *.dot; do dot -Tpng "$file" -o "${file%.dot}.png"; done

run: allp
	./$(PROGRAM)

