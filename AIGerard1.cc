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
    bool trol = true;
    vector<int> t = trolls();
    for(const auto& id : t){
      if(unit(id).pos == p) trol = false;
    }
    return cell(p).type != Abyss and cell(p).type != Granite and unit(balrog_id()).pos != p and trol and cell(p).owner != me();
  }

  bool condicions_wizard(Pos p){
    return cell(p).type != Abyss and cell(p).type != Granite and cell(p).type != Rock and unit(balrog_id()).pos != p;
  }

  bool buscar_dwarf(Pos p){
    bool dwarf = false;
    int id2 = cell(p).id;
    vector<int> d = dwarves(me());
    for(const auto& id : d){
      if(unit(id).type == Dwarf) dwarf = true;
    }
    return id2 != -1 and unit(id2).player == me() and dwarf;
  }

  Pos bfs_wizard(Pos p, vector<vector<int>>& dist, vector<vector<Pos>>& prev){
    dist[p.i][p.j] = 0;
    queue<Pos> Q;
    Q.push(p);
    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
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
    return Pos(-1, -1);
  }

  bool dwarf_accio(Pos p2, Pos p_ini){
    bool moure = false;
    if(cell(p2).id != -1 and cell(p2).owner != me() and unit(cell(p_ini).id).health >= unit(cell(p2).id).health) moure = true;
    else if(cell(p2).treasure) moure = true;
    return moure;
  }

  Pos bfs_dwarf(Pos p, vector<vector<int>>& dist, vector<vector<Pos>>& prev){
    dist[p.i][p.j] = 0;
    queue<Pos> Q;
    Q.push(p);
    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
      for(int i = 0; i < 8; ++i){
        Pos p2 = u + Dir(i);
        if(pos_ok(p2) and dist[p2.i][p2.j] == inf and condicions_dwarf(p2)){
          Q.push(p2);
          dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
          prev[p2.i][p2.j] = u;
          if(dwarf_accio(p2, p)) return p2;
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
      Pos p = bfs_dwarf(unit(id).pos, dist, prev);
      if (p != Pos(-1, -1)){
        int i = p.i;
        int j = p.j;
        while(dist[i][j] > 1){
          Pos p_pre = prev[i][j];
          i = p_pre.i;
          j = p_pre.j;
        }
        int i2 = i - unit(id).pos.i;
        int j2 = j - unit(id).pos.j;
        if(i2 == 1 and j2 == 0) {
          command(id, Bottom);
        }
        else if(i2 == 1 and j2 == 1){
          command(id, BR);
        }
        else if(i2 == -1 and j2 == 0) {
          command(id, Top);
        }
        else if(i2 == -1 and j2 == -1) {
          command(id, TL);
        }
        else if(i2 == 0 and j2 == 1) {
          command(id, Right);
        }
        else if(i2 == -1 and j2 == 1){
          command(id, RT);
        }
        else if(i2 == 0 and j2 == -1) {
          command(id, Left);
        }
        else if(i2 == 1 and j2 == -1) {
          command(id, LB);
        }
      }
      else {
        command(id, None);
      }
    }
  }

  void moure_wizard(){
    vector<int> w = wizards(me());
    for(auto& id : w){
      vector<vector<int>> dist(60, vector<int>(60,inf));
      vector<vector<Pos>> prev(60, vector<Pos>(60, Pos(-1, -1)));
      Pos p = bfs_wizard(unit(id).pos, dist, prev);
      if (p != Pos(-1, -1)){
        int i = p.i;
        int j = p.j;
        while(dist[i][j] > 1){
          Pos p_pre = prev[i][j];
          i = p_pre.i;
          j = p_pre.j;
        }
        int i2 = i - unit(id).pos.i;
        int j2 = j - unit(id).pos.j;
        if(i2 == 1 and j2 == 0) {
          command(id, Bottom);
        }
        else if(i2 == -1 and j2 == 0) {
          command(id, Top);
        }
        else if(i2 == 0 and j2 == 1) {
          command(id, Right);
        }
        else if(i2 == 0 and j2 == -1) {
          command(id, Left);
        }
      }
      else {
        command(id, None);
      }
    }
  }

  virtual void play () {
    moure_dwarf();
    moure_wizard();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
