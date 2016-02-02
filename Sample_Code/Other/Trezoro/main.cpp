#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum State { MAINMENU, GAME, QUIT };

struct Coords
{
    int x, y;
};

struct Board
{
    char tiles[20][10];
};

void ClearScreen();
int GetIntInput( int minimum, int maximum );
string GetStringInput();
State MainMenu();
State Gameplay();
void GenerateMap( Board& board, Coords& player, Coords& treasure );
void DrawMap( const Board& board, const Coords& player, const Coords& treasure );
void DrawHud();
void MovePlayer( Coords& player, const Board& board, string direction );
State YouWin();

int main()
{
    srand( time( NULL ) );
    State state = MAINMENU;

    while ( state != QUIT )
    {
        if ( state == MAINMENU )
        {
            state = MainMenu();
        }
        else
        {
            state = Gameplay();
        }
    }

    cout << endl << "Goodbye" << endl;

    return 0;
}

State MainMenu()
{
    ClearScreen();
    cout << "TTTTT RRR   EEEE  ZZZZZ  OOOO  RRR    OOOO" << endl;
    cout << "  T   R  R  E        Z   O  O  R  R   O  O" << endl;
    cout << "  T   RRR   EEEE   ZZ    O  O  RRR    O  O" << endl;
    cout << "  T   R  R  E     Z      O  O  R  R   O  O" << endl;
    cout << "  T   R   R EEEE  ZZZZZ  OOOO  R   R  OOOO" << endl;
    cout << "------------------------------------------" << endl;
    cout << "1. Play" << endl;
    cout << "2. Instructions" << endl;
    cout << "3. Quit" << endl;
    int choice = GetIntInput( 1, 3 );

    if ( choice == 1 )
    {
        return GAME;
    }
    else if ( choice == 3 )
    {
        return QUIT;
    }
    else
    {
        cout << "Use the [W, A, S, D] keys to move around the labyrinth." << endl;
        cout << "Find treasure and collect it to win." << endl;
        cout << endl;
        cout << "Go back? (y/n): ";
        GetStringInput();
        return MAINMENU;
    }
}

State Gameplay()
{
    bool done = false;
    Board board;
    Coords player, treasure;
    GenerateMap( board, player, treasure );

    while ( !done )
    {
        ClearScreen();
        DrawMap( board, player, treasure );
        DrawHud();
        string choice = GetStringInput();
        MovePlayer( player, board, choice );

        if ( choice == "QUIT" )
        {
            done = true;
        }

        if ( player.x == treasure.x && player.y == treasure.y )
        {
            return YouWin();
        }
    }

    return MAINMENU;
}

void ClearScreen()
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    system( "cls" );
    #else
    system( "clear" );
    #endif
}

int GetIntInput( int minimum, int maximum )
{
    cout << ">> ";
    int val;
    cin >> val;
    while ( val < minimum || val > maximum )
    {
        cout << "Invalid choice." << endl;
        cout << ">> ";
        cin >> val;
    }
    return val;
}

string GetStringInput()
{
    cout << ">> ";
    string val;
    cin >> val;
    return val;
}

void DrawMap( const Board& board, const Coords & player, const Coords & treasure )
{
    for ( int y = 0; y < 10; y++ )
    {
        for ( int x = 0; x < 20; x++ )
        {
            if ( x == player.x && y == player.y )
            {
                cout << "@";
            }
            else if ( x == treasure.x && y == treasure.y )
            {
                cout << "$";
            }
            else
            {
                cout << board.tiles[x][y];
            }
        }
        cout << endl;
    }
}

void GenerateMap( Board& board, Coords& player, Coords& treasure )
{
    Coords startingPoint;
    // Fill in all the rooms
    for ( int y = 0; y < 10; y++ )
    {
        for ( int x = 0; x < 20; x++ )
        {
            board.tiles[x][y] = '#';
        }
    }

    vector<Coords> roomCoords;

    // Choose random rooms
    int roomCount = rand() % 5 + 5;
    for ( int i = 0; i < roomCount; i++ )
    {
        Coords c;
        c.x = rand() % 20;
        c.y = rand() % 10;
        board.tiles[c.x][c.y] = '.';
        roomCoords.push_back( c );
    }

    player.x = roomCoords[0].x;
    player.y = roomCoords[0].y;
    treasure.x = roomCoords[ roomCount-1 ].x;
    treasure.y = roomCoords[ roomCount-1 ].y;

    // Connect all the rooms
    for ( int i = 0; i < roomCoords.size() - 1; i++ )
    {
        int x = roomCoords[i].x;
        int y = roomCoords[i].y;
        int targetX = roomCoords[i+1].x;
        int targetY = roomCoords[i+1].y;

        while ( x != targetX )
        {
            board.tiles[x][y] = '.';

            if ( x < targetX ) { x++; }
            else { x--; }
        }

        while ( y != targetY )
        {
            board.tiles[x][y] = '.';

            if ( y < targetY ) { y++; }
            else { y--; }
        }
    }
}

void DrawHud()
{
    cout << endl;
    cout << "[N]orth, [S]outh, [E]ast, or [W]est, or QUIT to quit." << endl;
}

void MovePlayer( Coords& player, const Board& board, string direction )
{
    char firstLetter = direction[0];
    firstLetter = tolower( firstLetter );

    if ( firstLetter == 'n' )
    {
        if ( player.y - 1 >= 0 && board.tiles[player.x][player.y - 1] == '.' )
        {
            player.y--;
        }
    }
    else if ( firstLetter == 's' )
    {
        if ( player.y + 1 < 10 && board.tiles[player.x][player.y + 1] == '.' )
        {
            player.y++;
        }
    }
    else if ( firstLetter == 'e' )
    {
        if ( player.x + 1 < 20 && board.tiles[player.x + 1][player.y] == '.' )
        {
            player.x++;
        }
    }
    else if ( firstLetter == 'w' )
    {
        if ( player.x - 1 >= 0 && board.tiles[player.x - 1][player.y] == '.' )
        {
            player.x--;
        }
    }
}

State YouWin()
{
    ClearScreen();
    cout << "YOU WIN!" << endl;
    cout << "You found the treasure!" << endl;
    cout << endl;
    cout << "Go back to main menu? (y/n): ";
    string input = GetStringInput();
    if ( input == "n" )
    {
        return QUIT;
    }
    else
    {
        return MAINMENU;
    }
}
