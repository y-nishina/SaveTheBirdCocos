//
//  Stage.cpp
//  SaveTheBird
//
//  Created by giginet on 7/9/14.
//
//

#include "Stage.h"

USING_NS_CC;

Stage::Stage()
: _level(0)
, _tiledMap(nullptr)
, _player(nullptr)
{
}

Stage::~Stage()
{
    CC_SAFE_RELEASE_NULL(_tiledMap);
    CC_SAFE_RELEASE_NULL(_player);
}

Stage * Stage::createWithLevel(int level)
{
    Stage *ret = new Stage();
    if (ret->initWithLevel(level))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Stage::initWithLevel(int level)
{
    if (!Layer::init())
    {
        return false;
    }
    
    // 現在のステージ番号をセット
    _level = level;
    
    auto winSize = Director::getInstance()->getWinSize();
    auto map = TMXTiledMap::create(StringUtils::format("map/stage%d.tmx", level));
    
    if (Director::getInstance()->getContentScaleFactor() == 1.0) {
        map->setScale(0.9);
    }
    
    this->addChild(map);
    
    this->setTiledMap(map);
    
    // 地形レイヤーを取得する
    auto terrainLayer = map->getLayer("Terrain");
    // オブジェクトレイヤーを取得する
    auto objectLayer = map->getLayer("Object");
    
    // マップのサイズ
    auto mapSize = map->getMapSize();
    for (int x = 0; x < mapSize.width; ++x) {
        for (int y = 0; y < mapSize.height; ++y) {
            auto coordinate = Vec2(x, y);
            
            if (terrainLayer != NULL) {
                this->addPhysicsBody(terrainLayer, coordinate);
            }
            if (objectLayer != NULL) {
                this->addPhysicsBody(objectLayer, coordinate);
            }
            
        }
    }
    
    // Playerの生成 (開始位置)
    auto player = Player::create();
    player->setPosition(Vec2(_tiledMap->getContentSize().width/2 , _tiledMap->getContentSize().height / 2));
    if (Director::getInstance()->getContentScaleFactor() == 1.0) {
        player->setScale(0.85f);
    }
    this->addChild(player);
    this->setPlayer(player);
    
    Point origin = Director::sharedDirector()->getVisibleOrigin();
    Size size = Director::sharedDirector()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
    Point center = *new Point(size.width/2 + origin.x, size.height/2 + origin.y);
    
    // Playerの移動に画面を追従させる
    this->runAction(Follow::create(player, Rect((center.x - _tiledMap->getContentSize().width / 2),
                                                (center.y - _tiledMap->getContentSize().height / 2 -80),
                                                _tiledMap->getContentSize().width * 2 - center.x * 2,
                                                _tiledMap->getContentSize().height * 2 - center.y * 2 + 160
                                                )));

    this->scheduleUpdate();
    
    return true;
}

void Stage::update(float dt)
{
}

Sprite* Stage::addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2 &coordinate)
{
    // タイル1枚の大きさを取り出す
    auto tileSize = _tiledMap->getTileSize();
    
    // タイルのスプライトを取り出す
    auto sprite = layer->getTileAt(coordinate);
    if (sprite) {
        // タイルのIDを取り出す
        auto gid = layer->getTileGIDAt(coordinate);
        // タイルのプロパティをmapで取り出す
        auto property = _tiledMap->getPropertiesForGID(gid);
        if (property.isNull() || property.getType() != Value::Type::MAP) {
            return nullptr;
        }
        auto properties = property.asValueMap();
        // プロパティの中からcategoryの値をintとして取り出す
        auto category = properties.at("category").asInt();
        
        auto material = PhysicsMaterial();
        material.friction = 0;
        material.restitution = 0.1;
        
        // 剛体を設置する
        auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), material);
        // 剛体を固定する
        physicsBody->setDynamic(false);
        // 剛体にカテゴリをセットする
        physicsBody->setCategoryBitmask(category);
        // 剛体と接触判定を取るカテゴリを指定する
        physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
        // 剛体と衝突を取るカテゴリを指定する
        physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
        
        // アニメーションを付ける
        // プロパティにanimationの値があったら
        if (!properties["animation"].isNull()) {
            auto animationSprite = properties["animation"].asString();
            auto animationCount = properties["animationCount"].asInt();
        
            sprite->removeFromParent();
            this->addChild(sprite);
            
            Vector<SpriteFrame *> frames;
            auto scale = 0.5;
            for (int i = 0; i < animationCount; ++i) {
                auto frame = SpriteFrame::create(animationSprite, Rect(tileSize.width * i * scale, 0, tileSize.width * scale, tileSize.height * scale));
                frames.pushBack(frame);
            }
            auto animation = Animation::createWithSpriteFrames(frames);
            animation->setDelayPerUnit(0.15);
            sprite->runAction(RepeatForever::create(Animate::create(animation)));
        }
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPhysicsBody(physicsBody);
        
        return sprite;
    }
    return nullptr;
}