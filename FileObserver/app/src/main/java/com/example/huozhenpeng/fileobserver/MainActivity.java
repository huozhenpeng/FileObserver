package com.example.huozhenpeng.fileobserver;

import android.os.Build;
import android.os.Environment;
import android.os.FileObserver;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private  String path;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
//         path= getCacheDir()+ File.separator+"observer.txt";
        path="/data/data/com.example.huozhenpeng.fileobserver";
        File file=new File(path);
        if(!file.exists())
        {
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }

        }
        FileObserver fileObserver=new FileObserver(path,FileObserver.MODIFY) {
            @Override
            public void onEvent(int event, String path) {

                Log.e("abc","文件有修改");
            }
        };
//        fileObserver.startWatching();
//        fileObserver.stopWatching();
        stringFromJNI(path, Build.VERSION.SDK_INT);
        tv.setText("abc");
    }

    public void write(View view)
    {
        writeFile(path,"文本");
    }

    BufferedWriter out = null;
    public  void writeFile(String file, String conent) {

        try {
            out = new BufferedWriter(new OutputStreamWriter(
                    new FileOutputStream(file, true)));
            out.write(conent);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                out.close();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


        /**
         * A native method that is implemented by the 'native-lib' native library,
         * which is packaged with this application.
         */
    public native void stringFromJNI(String path,int sdk);
}
