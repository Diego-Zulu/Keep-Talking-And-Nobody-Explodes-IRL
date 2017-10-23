from game.Game import *

salir = False
while(not(salir)):
    print("1 - Start Game")
    print("0 - Exit")
    p = input()
    if p == "1":
        game = Game()
        game.connect("192.168.43.204")
        game.start()
        salirJuego = False
        while(not(salirJuego)):
            print("1 - End Game")
            p = input()
            if p == "1":
                game.end()
                salirJuego = True
            if p == "2":
                game.w.send_message("ERRFDFDDGFDOR");
    if p == "0":
        salir = True