gcc src/Respcli.c -o Respcli

gcc -Iinclude src/Server.c src/Cmdlexicon.c src/HTable.c src/Resp.c -o Server