package com.aaronbot3000.pythagorasandroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;

public class StageOne extends Activity {
    /** Called when the activity is first created. */
	
	private final int CAMERA_INTENT_ID = 1;

	private ImageView display;
	private SeekBar seekbar_blur;
	private SeekBar seekbar_high_thresh;
	private SeekBar seekbar_low_thresh;
	
	private int blur_amt;
	private int high_amt;
	private int low_amt;
	
	private boolean has_image = false;
	
	private class SeekbarChangeCallback implements SeekBar.OnSeekBarChangeListener {
		public void onProgressChanged(SeekBar sb, int val, boolean fromUser) {
			if (sb.getId() == R.id.slider_blur) {
				blur_amt = val;
			}
			else if (sb.getId() == R.id.slider_high_thresh) {
				high_amt = val;
			}
			if (sb.getId() == R.id.slider_low_thresh) {
				low_amt = val;
			}
			if (has_image)
				updateDisplay();
		}

		public void onStartTrackingTouch(SeekBar arg0) {}
		public void onStopTrackingTouch(SeekBar arg0) {}		
	}
	
	private class ButtonCallback implements View.OnClickListener {
        public void onClick(View view) {
        	if (view.getId() == R.id.button_continue)
        		startStageTwo();
        	else if (view.getId() == R.id.button_take_picture)
        		startCameraActivity();
        }
    }
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        seekbar_blur = (SeekBar)findViewById(R.id.slider_blur);
        seekbar_blur.setMax(10);
        seekbar_high_thresh = (SeekBar)findViewById(R.id.slider_high_thresh);
        seekbar_low_thresh = (SeekBar)findViewById(R.id.slider_low_thresh);
        
        SeekbarChangeCallback scc = new SeekbarChangeCallback();
        seekbar_blur.setOnSeekBarChangeListener(scc);
        seekbar_high_thresh.setOnSeekBarChangeListener(scc);
        seekbar_low_thresh.setOnSeekBarChangeListener(scc);
        
        ButtonCallback bc = new ButtonCallback();
        ((Button)findViewById(R.id.button_continue)).setOnClickListener(bc);
        ((Button)findViewById(R.id.button_take_picture)).setOnClickListener(bc);
    }
    
    protected void startCameraActivity() {
        Intent intent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(intent, CAMERA_INTENT_ID);
    }
    
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        switch( resultCode )
        {
        	case 0:
        		Log.i( "MakeMachine", "User cancelled" );
        		break;

        	case -1:
        		onPhotoTaken();
        		break;
        }
    }

    private void updateDisplay() {
    	
    }
}