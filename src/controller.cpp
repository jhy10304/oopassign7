#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"

//比較座標、for()檢查每一個座標

Controller::Controller(View& view) : _view(view){

    _objs.emplace_back(SimpleGameObjectFactory::playerGameObject());
    for(int i = 0; i < 5; ++i){
        _objs.emplace_back(SimpleGameObjectFactory::randomGameObject());
    }
}
bool Controller::collision(){
    for(int i=1;i<_objs.size();i++){
        if(_objs[0]->getPosition()==_objs[i]->getPosition()){
            return true;
        }
    }
    return false;
}
bool Controller::getstatus(){
    return status;
}
void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    // init state
    int input = -1;
    clock_t start, end;
    
    // Main loop
    while (true) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27)break;
        if(this->collision()==true){
            status =true;
            break;

        }
        this->handleInput(input);

        _view.resetLatest();
        for(GameObject* obj : _objs) 
        {

            obj->update();

            _view.updateGameObject(obj);
        }
       
        _view.render();
        end = clock();
        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
    }

}



void Controller::handleInput(int keyInput){
    if(keyInput==-1)return;
    
    if(keyInput == 'w' || keyInput == 'W'){
        _objs[0]->setDirection(UP);
        //generate 1 obstacle
        _objs.emplace_back(SimpleGameObjectFactory::randomGameObjectWithpara(_objs[0]->getPosition()));
    }

    if(keyInput == 'a' || keyInput == 'A'){

        _objs[0]->setDirection(LEFT);
        _objs.emplace_back(SimpleGameObjectFactory::randomGameObjectWithpara(_objs[0]->getPosition()));
    }

    if(keyInput == 's' || keyInput == 'S'){

        _objs[0]->setDirection(DOWN);
        _objs.emplace_back(SimpleGameObjectFactory::randomGameObjectWithpara(_objs[0]->getPosition()));
    }

    if(keyInput == 'd' || keyInput == 'D'){

        _objs[0]->setDirection(RIGHT);
        _objs.emplace_back(SimpleGameObjectFactory::randomGameObjectWithpara(_objs[0]->getPosition()));
    }

    if(keyInput == '\t'){
    }
}

void Controller::update(){

}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

