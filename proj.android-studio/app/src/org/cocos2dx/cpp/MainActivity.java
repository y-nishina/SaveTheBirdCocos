package org.cocos2dx.cpp;

/**
 * Created by arimatakeshi on 16/05/16.
 */

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Typeface;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.Window;
import android.widget.ImageButton;
import android.widget.TextView;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.util.List;


public class MainActivity extends Activity{

    private HomeFragment homeFragment;
    private CharacterSelectFragment characterSelectFragment;
    private SettingFragment settingFragment;
    public List<CharacterData> characterDataList;
    private ImageButton homeButton;
    private ImageButton characterButton;
    private ImageButton settingButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // タイトル消し
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        // タブバーのインスタンス化
        homeButton = (ImageButton)findViewById(R.id.homeBtn);
        characterButton = (ImageButton)findViewById(R.id.characterBtn);
        settingButton = (ImageButton)findViewById(R.id.settingBtn);

        // 初期タブ（ホーム）の画像を変更
        homeButton.setImageResource(R.mipmap.ic_menu_home_on_66);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // DBからキャラクターのデータを取得
        SaveTheBirdDBOpenHelper dbHelper = new SaveTheBirdDBOpenHelper(this);
        SQLiteDatabase db = dbHelper.getReadableDatabase();
        CharacterDao characterDao = new CharacterDao(db);
        characterDataList = characterDao.findAll();

        // BGMを再生するかどうかのフラグを読み取る
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
        boolean bgmFlg = preferences.getBoolean("bgm_flg", true);
        if (bgmFlg){
            // サービスを起動してBGMの再生を開始
            startService(new Intent(this, BgmService.class));
        }

        // フラグメントの初期化
        initFragment();
    }

    private void initFragment(){
        // フラグメントのインスタンスを生成
        if (homeFragment == null){
            homeFragment = new HomeFragment();
        }
        if (characterSelectFragment == null){
            characterSelectFragment = new CharacterSelectFragment();
        }
        if (settingFragment == null){
            settingFragment = new SettingFragment();
        }

        // フラグメントをViewに追加
        FragmentManager manager = getFragmentManager();
        FragmentTransaction transaction = manager.beginTransaction();
        Fragment currentFragment = manager.findFragmentById(R.id.fragment_container);
        if (currentFragment == null){
            transaction.add(R.id.fragment_container,homeFragment);
            transaction.add(R.id.fragment_container,characterSelectFragment);
            transaction.add(R.id.fragment_container,settingFragment);

            transaction.hide(characterSelectFragment);
            transaction.hide(settingFragment);
            transaction.show(homeFragment);
        }
        transaction.commit();
    }

    // タブメニュー操作
    public void tabClick(View view){

        FragmentManager manager = getFragmentManager();
        FragmentTransaction transaction = manager.beginTransaction();

        switch (view.getId()){

            case R.id.homeBtn:
                if (!homeFragment.isVisible()){
                    // フラグメントの切り替え
                    transaction.hide(characterSelectFragment);
                    transaction.hide(settingFragment);
                    transaction.show(homeFragment);
                    // タブバーの画像を切り替え
                    characterButton.setImageResource(R.mipmap.ic_menu_chara_66);
                    settingButton.setImageResource(R.mipmap.ic_menu_setting_66);
                    homeButton.setImageResource(R.mipmap.ic_menu_home_on_66);

                    // キャラクターの画像を更新
                    homeFragment.changeCharacter();
                    //
                }
                break;

            case R.id.characterBtn:
                if (!characterSelectFragment.isVisible()){
                    // フラグメントの切り替え
                    transaction.hide(homeFragment);
                    transaction.hide(settingFragment);
                    transaction.show(characterSelectFragment);
                    // タブバーの画像を切り替え
                    homeButton.setImageResource(R.mipmap.ic_menu_home_66);
                    settingButton.setImageResource(R.mipmap.ic_menu_setting_66);
                    characterButton.setImageResource(R.mipmap.ic_menu_chara_on_66);
                }
                break;

            case R.id.settingBtn:
                if (!settingFragment.isVisible()){
                    // フラグメントの切り替え
                    transaction.hide(homeFragment);
                    transaction.hide(characterSelectFragment);
                    transaction.show(settingFragment);
                    // タブバーの画像を切り替え
                    homeButton.setImageResource(R.mipmap.ic_menu_home_66);
                    characterButton.setImageResource(R.mipmap.ic_menu_chara_66);
                    settingButton.setImageResource(R.mipmap.ic_menu_setting_on_66);
                }
                break;

            default:
                break;

        }
        transaction.commit();
    }

    /*
    * フォントタイプ指定
    * setFontType()
    */
    public void setFontType(TextView txt) {
        txt.setTypeface(Typeface.createFromAsset(getAssets(), "fonts/logotypejp_mp_m_1_1.ttf"));
    }

    /*
    * Assetフォルダから画像をBitmap形式で取得する
    * loadBitmapFromAsset(String fileName)
     */
    public Bitmap loadBitmapFromAsset(String fileName)throws IOException{
        final AssetManager assetManager = getAssets();
        BufferedInputStream bis = null;

        try {
            bis = new BufferedInputStream(assetManager.open(fileName));
            return BitmapFactory.decodeStream(bis);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (bis != null) {
                    bis.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    @Override
    protected void onPause() {
        super.onPause();

        // BGMを再生しているサービスを停止する
        stopService(new Intent(this, BgmService.class));
    }
}
