package com.aaronbot3000.pythagorasandroid;

import java.io.File;
import java.io.IOException;

import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
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

	private static boolean has_image = false;

	private static Bitmap imagedata, cvdisplay = null;
	
	private static File tempfile = null;

	private class SeekbarChangeCallback implements
			SeekBar.OnSeekBarChangeListener {
		public void onProgressChanged(SeekBar sb, int val, boolean fromUser) {
			if (sb.getId() == R.id.slider_blur) {
				blur_amt = val;
			} else if (sb.getId() == R.id.slider_high_thresh) {
				high_amt = val;
			}
			if (sb.getId() == R.id.slider_low_thresh) {
				low_amt = val;
			}
			if (has_image)
				updateDisplay();
		}

		public void onStartTrackingTouch(SeekBar arg0) {
		}

		public void onStopTrackingTouch(SeekBar arg0) {
		}
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

		seekbar_blur = (SeekBar) findViewById(R.id.slider_blur);
		seekbar_blur.setMax(10);
		seekbar_high_thresh = (SeekBar) findViewById(R.id.slider_high_thresh);
		seekbar_low_thresh = (SeekBar) findViewById(R.id.slider_low_thresh);

		SeekbarChangeCallback scc = new SeekbarChangeCallback();
		seekbar_blur.setOnSeekBarChangeListener(scc);
		seekbar_high_thresh.setOnSeekBarChangeListener(scc);
		seekbar_low_thresh.setOnSeekBarChangeListener(scc);

		display = (ImageView) findViewById(R.id.image_first_stage);
		//cvdisplay = Bitmap.createBitmap(width, height, config);

		ButtonCallback bc = new ButtonCallback();
		((Button) findViewById(R.id.button_continue)).setOnClickListener(bc);
		((Button) findViewById(R.id.button_take_picture))
				.setOnClickListener(bc);
		
		if (has_image) {
			display.setImageBitmap(imagedata);
		}
		
		if (tempfile == null) {
			try {
				tempfile = File.createTempFile("pythagorasinput", ".bmp");
			}
			catch(IOException e) {
				Log.e("pythagoras", "Cannot create temp file! FFFFFFFUUUUUUUUUUUU");
				e.printStackTrace();
			}
		}
	}
	
	protected void onDestroy() {
		super.onDestroy();
		
		if (imagedata != null)
			imagedata.recycle();
		
		if (cvdisplay != null) {
			cvdisplay.recycle();
			cvdisplay = null;
		}
		
		if (tempfile != null) {
			tempfile.delete();
			tempfile = null;
		}
	}

	private void startCameraActivity() {
		Intent intent = new Intent(
				android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
		intent.putExtra( MediaStore.EXTRA_OUTPUT, Uri.fromFile(tempfile));
		startActivityForResult(intent, CAMERA_INTENT_ID);
	}

	private void startStageTwo() {
	}

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (resultCode) {
		case 0:
			Log.i("MakeMachine", "User cancelled");
			break;

		case -1:
			BitmapFactory.Options options = new BitmapFactory.Options();
	        options.inSampleSize = 8;
	        
	        if (imagedata != null)
	        	imagedata.recycle();
			imagedata = (Bitmap) BitmapFactory.decodeFile(tempfile.getAbsolutePath(), options);
			Log.i("pythagoras", "image in type: " + imagedata.getConfig());
			imagedata = imagedata.copy(Bitmap.Config.ARGB_8888, false);
			has_image = true;
			updateDisplay();
			break;
		}
	}

	private void updateDisplay() {
		//display.setImageBitmap(imagedata);
		synchronized (this) {
			Mat cvmat = Utils.bitmapToMat(imagedata);
			Log.i("pythagoras", "made input mat");
			Mat gray = new Mat(), cvoutput = new Mat(), grayout = new Mat();
			Log.i("pythagoras", "alloced interim mats");
			Imgproc.cvtColor(cvmat, gray, Imgproc.COLOR_RGB2GRAY);
			Log.i("pythagoras", "conv to gray");
			Imgproc.GaussianBlur(gray, gray, new Size(new Point(blur_amt * 2 + 1,
					blur_amt * 2 + 1)), 10d);
			Log.i("pythagoras", "gaussian blurred");
			Imgproc.cvtColor(gray, cvoutput, Imgproc.COLOR_GRAY2RGBA);
			Log.i("pythagoras", "conv to color");
			
			if (cvdisplay == null)
				cvdisplay = Bitmap.createBitmap(cvoutput.width(), cvoutput.height(), Bitmap.Config.ARGB_8888);

			Utils.matToBitmap(cvoutput, cvdisplay);
			Log.i("pythagoras", "conv to bmp");
			display.setImageBitmap(cvdisplay);
		}
	}
}