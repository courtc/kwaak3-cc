package org.kwaak3;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;

public class Launcher extends Activity{
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		Button button = (Button)findViewById(R.id.btnStartGame);
		button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(v.getContext(),Game.class);
				CheckBox chk = (CheckBox)findViewById(R.id.chkEnableSound);
				intent.putExtra("sound", chk.isChecked());

				chk = (CheckBox)findViewById(R.id.chkShowFramerate);
				intent.putExtra("showfps", chk.isChecked());

				chk = (CheckBox)findViewById(R.id.chkEnableLightmaps);
				intent.putExtra("lightmaps", chk.isChecked());
				
				startActivity(intent);
			}});

		button = (Button)findViewById(R.id.btnRunBenchmark);
		button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(v.getContext(),Game.class);
				CheckBox chk = (CheckBox)findViewById(R.id.chkEnableSound);
				intent.putExtra("sound", chk.isChecked());
				
				chk = (CheckBox)findViewById(R.id.chkShowFramerate);
				intent.putExtra("showfps", chk.isChecked());

				chk = (CheckBox)findViewById(R.id.chkEnableLightmaps);
				intent.putExtra("lightmaps", chk.isChecked());			
				
				intent.putExtra("benchmark", true);
				startActivity(intent);
			}});
		
		
		button = (Button)findViewById(R.id.btnVisitWebsite);
		button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://code.google.com/p/kwaak3/"));
				startActivity(intent);
			}});
	}
}
