package cn.root4.opengles30ndk;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private GLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        FrameLayout fr = new FrameLayout(this);
        {
            FrameLayout.LayoutParams fl;

            mGLSurfaceView = new GLSurfaceView(this);
            mGLSurfaceView.setEGLContextClientVersion(3);
            mGLSurfaceView.setRenderer(new NativeRenderer(this, 0xff0000ff));
            //mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
            fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT);
            fr.addView(mGLSurfaceView, fl);

            Button btn = new Button(this);
            btn.setAlpha(0.3f);
            btn.setText("left");
            btn.setTextSize(20);
            fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
            fl.gravity = Gravity.LEFT | Gravity.CENTER_VERTICAL;
            fr.addView(btn, fl);
            btn.setOnClickListener((View view) -> onDirection(1));

            btn = new Button(this);
            btn.setAlpha(0.3f);
            btn.setText("up");
            btn.setTextSize(20);
            fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
            fl.gravity = Gravity.CENTER_HORIZONTAL | Gravity.TOP;
            fr.addView(btn, fl);
            btn.setOnClickListener((View view) -> onDirection(2));

            btn = new Button(this);
            btn.setAlpha(0.3f);
            btn.setText("right");
            btn.setTextSize(20);
            fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
            fl.gravity = Gravity.RIGHT | Gravity.CENTER_VERTICAL;
            fr.addView(btn, fl);
            btn.setOnClickListener((View view) -> onDirection(3));

            btn = new Button(this);
            btn.setAlpha(0.3f);
            btn.setText("bottom");
            btn.setTextSize(20);
            fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
            fl.gravity = Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM;
            fr.addView(btn, fl);
            btn.setOnClickListener((View view) -> onDirection(4));
        }
        setContentView(fr);
    }

    /**
     * @param d 1左、2上、3右、4下
     */
    private static native void onDirection(int d);
}
