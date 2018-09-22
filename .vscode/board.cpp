class Board
{
  public:
    int colorsNumber;
    int **board;
    Board *next;

    int weight=0;

    Board(int** board ,int colors){
        colorsNumber= colors;
    }
    
    void expand(){
        
    }


};