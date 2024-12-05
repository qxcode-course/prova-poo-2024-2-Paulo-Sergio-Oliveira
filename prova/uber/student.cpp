#include <iostream>
#include "fn.hpp"
using namespace std;

class Pessoa {
    string nome;
    int dinheiro;

    public:

        Pessoa (string nome, int dinheiro): nome(nome), dinheiro(dinheiro) {}

        string getNome() {
            return this->nome;
        }

        void setNome(string nome) {
            this->nome = nome;
        }

        int getDinheiro() {
            return this->dinheiro;
        }

        void setDinheiro(int dinheiro) {
            this->dinheiro = dinheiro;
        }

        string str() {
            return fn::format("{}:{}", this->nome, this->dinheiro);
        }
};

class Moto {
    int custo;
    shared_ptr<Pessoa> motorista;
    shared_ptr<Pessoa> passageiro;

    public:

        Moto(int custo, shared_ptr<Pessoa> motorista, shared_ptr<Pessoa> passageiro): 
        custo(custo), motorista(motorista), passageiro(passageiro) {}

        string str() {
            return fn::format("Cost: {}, Driver: {}, Passenger: {}\n",
            this->custo, (this->motorista != nullptr ? this->motorista->str() : "None"),
            (this->passageiro != nullptr ? this->passageiro->str() : "None"));
        }

        void setDriver(string nome, int dinheiro) {
            if(this->motorista == nullptr) {
                this->motorista = make_shared<Pessoa>(nome, dinheiro);
                return;
            }

            cout << "fail: There is already a driver" << endl;
        }

        void setPass(string nome, int dinheiro) { 
            if(this->passageiro == nullptr) {
                this->passageiro = make_shared<Pessoa>(nome, dinheiro);
                return;
            }

            cout << "fail: There is already a passenger" << endl;
        }

        void drive(int distancia) {
            if(this->motorista != nullptr) {
                this->custo += distancia;
            }
            
            cout << "fail: There is no driver to drive" << endl;
        }

        void leavePass() {
            if(this->passageiro != nullptr) {
                this->motorista->setDinheiro(this->motorista->getDinheiro() + this->custo);
                this->passageiro->setDinheiro(this->passageiro->getDinheiro() - this->custo);
                this->custo = 0;
                
                if (this->passageiro->getDinheiro() < 0) {
                    cout << "fail: Passenger does not have enough money" << endl;
                    this->passageiro->setDinheiro(0);
                }
                
                cout << this->passageiro->getNome() << ":" << this->passageiro->getDinheiro() << " leave" << endl;
                this->passageiro = nullptr;
                return;
            }

            cout << "fail: There is no passenger to leave" << endl;
        }
};

class Adapter {
public:
    Moto moto = Moto(0, nullptr, nullptr);

    void setDriver(string name, int money) {
        moto.setDriver(name, money);
    }
    void setPass(string name, int money) {
        moto.setPass(name, money);
    }
    void drive(int distance) {
        moto.drive(distance);
    }
    void leavePass() {
        moto.leavePass();
    }
    void show() {
        cout << moto.str();
    }
};

int main() {
    Adapter adp;
    while (true) {
        fn::write("$", "");
        auto line = fn::input();
        auto args = fn::split(line, ' ');
        fn::write(line);

        if      (args[0] == "end")       { break;                                  }
        else if (args[0] == "setDriver") { adp.setDriver(args[1], +args[2]);       }
        else if (args[0] == "setPass")   { adp.setPass(args[1], +args[2]);         }
        else if (args[0] == "drive")     { adp.drive(+args[1]);                    }
        else if (args[0] == "leavePass") { adp.leavePass();                        }
        else if (args[0] == "show")      { adp.show();                             }
        else                             { fn::write("fail: command not found\n"); }
    }
}
