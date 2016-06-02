//
//  Stage.h
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#ifndef __SaveTheBird__Stage__
#define __SaveTheBird__Stage__

#include "cocos2d.h"
#include "Player.h"

class Stage :public cocos2d::Layer
{
public:
    
    /// タイルの種類
    enum class TileType {
        /// 壁
        WALL = 1 << 0,
        /// プレイヤー
        PLAYER = 1 << 1,
        /// 敵キャラ
        ENEMY = 1 << 2,
        /// コイン
        COIN = 1 << 3,
        /// 収集アイテム
        ITEN = 1 << 4
    };
    
    void update(float dt) override;

    CC_SYNTHESIZE_READONLY(int, _level, Level);
    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
    CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
    
    /** ステージ番号からステージを生成します
     *  @param level ステージ番号
     *  @return ステージ
     */
    static Stage * createWithLevel(int level);
protected:
    bool initWithLevel(int level);
    Stage();
    virtual ~Stage();

private:
    /** 指定のレイヤーの特定位置のタイルに剛体を設置します
     *  指定座標にタイルがなかった場合はnullptrを返します
     *  @param layer 対象のレイヤー
     *  @param coordinate 対象タイルのあるマップ上の座標
     *  @return タイルのスプライト、またはnullptr
     */
    cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate);
    
};

#endif /* defined(__SaveTheBird__Stage__) */
