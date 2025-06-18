#include "gameObject.h"

struct SimpleGameObjectFactory {
    
    static GameObject* playerGameObject();
    static GameObject* randomGameObject();
    static GameObject* randomGameObjectWithpara(const Position& playerPos);
};
