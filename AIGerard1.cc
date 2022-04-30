#include "Player.hh"
#include <limits>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Gerard1


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  const int inf = numeric_limits<int>::max();
  
  bool condicions_dwarf(Pos p){
    return cell(p).type != Abyss and cell(p).type != Granite;
  }

  bool condicions_wizard(Pos p){
    return cell(p).type != Abyss and cell(p).type != Granite and cell(p).type != Rock;
  }

  bool buscar_dwarf(Pos p){
    int id2 = cell(p).id;
    return id2 != -1 and unit(id2).player == me();
  }

  Pos bfs(Pos p, int ut, vector<vector<int>>& dist, vector<vector<Pos>>& prev){
    dist[p.i][p.j] = 0;

    queue<Pos> Q;
    Q.push(p);

    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
      //Si ut==0 vol dir que és un Dwarf en cas contrari és un Wizard.
      if(ut==0){
        for(int i = 0; i < 8; ++i){
          Pos p2 = u + Dir(i);
          if(pos_ok(p2) and dist[p2.i][p2.j] == inf and condicions_dwarf(p2)){
            Q.push(p2);
            dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
            prev[p2.i][p2.j] = u;
            if(cell(p2).treasure) return p2;
          }
        }
      }
      else{
        for(int i = 0; i < 8; i+=2){
          Pos p2 = u + Dir(i);
          if(pos_ok(p2) and dist[p2.i][p2.j] == inf and condicions_wizard(p2)){
            Q.push(p2);
            dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
            prev[p2.i][p2.j] = u;
            if(buscar_dwarf(p2)) return p2;
          }
        }
      }
    }
    return Pos(-1, -1);
  }

  void moure_dwarf(){
    vector<int> d = dwarves(me());
    for(auto& id : d){
      vector<vector<int>> dist(60, vector<int>(60,inf));
      vector<vector<Pos>> prev(60, vector<Pos>(60, Pos(-1, -1)));
      Pos p = bfs(unit(id).pos, 0, dist, prev);
      cerr << p << endl;
      if (p != Pos(-1, -1)){
        int i, j;
        i = p.i;
        j= p.j;
        while(dist[i][j] > 1){
          Pos p_pre = prev[i][j];
          i = p_pre.i;
          j = p_pre.j;
        }
        int x = p.i - i;
        int y = p.j - j;
        if(x == 1 and y == 0) command(id, Bottom);
        else if(x == -1 and y == 0) command(id, Top);
        else if(x == 0 and y == 1) command(id, Right);
        else if(x == 0 and y == -1) command(id, Left);
      }
      else command(id, None);
    }
  }

  virtual void play () {
    moure_dwarf();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
