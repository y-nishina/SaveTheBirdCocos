#include "AppDelegate.h"
#include "TitleScene.h"
#include "MainScene.h"

//using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
/*void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}*/

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
/*static int register_all_packages()
{
    return 0; //flag for packages manager
}*/

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    // turn on display FPS
    director->setDisplayStats(false);


    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 30);
    
    auto fu = FileUtils::getInstance();
    
    // 画像リソースを振り分ける
    // 画像リソースを検索する順番を定義する配列
    std::vector<std::string> searchResolutionOrder;
    auto platform = this->getTargetPlatform(); // 現在のプラットフォーム
    auto frameSize = glview->getFrameSize(); // 現在の端末の画面サイズ
    
    if (platform == Platform::OS_IPHONE) { // もし、iPhoneなら
        fu->addSearchPath("music/caf");
        fu->addSearchPath("se/caf");
        if (frameSize.height > 320.f) {
            // Retinaディスプレイのとき
            // 高解像度画像を有効にする
            director->setContentScaleFactor(2.0f);
            if (frameSize.width == 1136) {
                // iPhone 4inchのとき
                // 4インチ対応の画面サイズに変更する
                //glview->setDesignResolutionSize(568, 320, ResolutionPolicy::NO_BORDER);
                glview->setDesignResolutionSize(320, 568, ResolutionPolicy::SHOW_ALL);
                // Resources/4inchフォルダに画像ファイルがあれば、最優先で利用する
                searchResolutionOrder.push_back("images/4inch");
            } else {
                // Retina 3.5インチのとき
                //glview->setDesignResolutionSize(480, 320, ResolutionPolicy::NO_BORDER);
                glview->setDesignResolutionSize(320, 480, ResolutionPolicy::SHOW_ALL);
            }
            searchResolutionOrder.push_back("images/retina");
        } else { // non-Retina 3.5インチ
            //glview->setDesignResolutionSize(480, 320, ResolutionPolicy::NO_BORDER);
            glview->setDesignResolutionSize(320, 480, ResolutionPolicy::SHOW_ALL);
            searchResolutionOrder.push_back("images/nonretina");
        }
    //} else if (platform == Platform::OS_IPAD) {
    //    fu->addSearchPath("music/caf");
    //    fu->addSearchPath("se/caf");
    //    if (frameSize.width > 768.f) {
    //        director->setContentScaleFactor(2.0f);
    //        searchResolutionOrder.push_back("images/retina");
    //    } else {
    //        searchResolutionOrder.push_back("images/nonretina");
    //    }
    //    glview->setDesignResolutionSize(568, 320, ResolutionPolicy::SHOW_ALL);
    } else if (platform == Platform::OS_ANDROID) {
        // Android端末のとき
        //glview->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);
        glview->setDesignResolutionSize(320, 480, ResolutionPolicy::SHOW_ALL);
        fu->addSearchPath("music/ogg");
        fu->addSearchPath("se/ogg");
        searchResolutionOrder.push_back("images/nonretina");
    }
    // 画像の読み込み順を設定する
    FileUtils::getInstance()->setSearchResolutionsOrder(searchResolutionOrder);
    
    // create a scene. it's an autorelease object
    auto scene = TitleScene::createScene();
    //auto scene = MainScene::createSceneWithStage(0);

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
