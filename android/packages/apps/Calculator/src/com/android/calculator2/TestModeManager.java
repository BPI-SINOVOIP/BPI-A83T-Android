package com.android.calculator2;

import java.io.File;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

public class TestModeManager {
	
	public static boolean start(Context context, String inputKey) {
		if(inputKey == null) return false;
		if(inputKey.equals(TEST_MODE_KEY) || inputKey.equals(TEST_MODE_CONFIG)) {
			boolean isStart = checkAndStart(context, inputKey);
			boolean isStartConfig = checkAndStartConfig(context, inputKey);
			if(isStart || isStartConfig) {
				return true;
			}
		} else if (inputKey.equals(TEST_MODE_START_LOGSERVICE) && (android.os.Process.myUid()/100000) == 0){
			startLogService(context);
			return true;
		}
		
		return false;
	}

	private static boolean checkAndStart(Context context, String inputKey) {
		boolean b = false;
		if (inputKey.equals(TEST_MODE_KEY) && (new File(FLAG_USBHOST_CONFIG).exists() || new File(FLAG_EXTSD_CONFIG).exists()
				|| new File(FLAG_SDCARD_CONFIG).exists())) {
			Intent i = new Intent();
			ComponentName component = new ComponentName(
					"com.softwinner.dragonfire",
					"com.softwinner.dragonfire.SplashScreen_test");

			i.setComponent(component);
			try {
				context.startActivity(i);
				b = true;
			} catch (Exception e) {
			}
		}
		return b;
	}

	private static boolean checkAndStartConfig(Context context, String inputKey) {
		boolean b = false;
		if (inputKey.equals(TEST_MODE_CONFIG) && (new File(FLAG_USBHOST_CONFIG).exists() || new File(FLAG_EXTSD_CONFIG).exists()
				|| new File(FLAG_SDCARD_CONFIG).exists())) {
			Intent i = new Intent();
			ComponentName component = new ComponentName(
					"com.softwinner.dragonfire",
					"com.softwinner.dragonfire.SplashScreen_configuration");
			i.setComponent(component);
			try {
				context.startActivity(i);
				b = true;
			} catch (Exception e) {
			}
		}
		return b;
	}
	
	private static void startLogService(Context context){
		if(context==null || !checkPackage(context, "com.softwinner.loggerservice")){
			return;
		}
		Intent i = new Intent();
		ComponentName component = new ComponentName(
				"com.softwinner.loggerservice",
				"com.softwinner.loggerservice.MainActivity");

		i.setComponent(component);
		try {
			context.startActivity(i);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
    private static boolean checkPackage(Context context, String packageName)
    { 
        if (packageName == null || "".equals(packageName)) return false; 
        try
        { 
        	context.getPackageManager().getApplicationInfo(packageName, PackageManager.GET_UNINSTALLED_PACKAGES); 
            return true; 
        }
        catch (NameNotFoundException e)
        { 
            return false; 
        } 
    }

	public final static String TEST_MODE_KEY = "33"; // check if input value =
														// dragonFireKey,
														// startup DragongFire
														// Main Activity.
	public final static String TEST_MODE_CONFIG = "23"; // if input value = 23,
														// startup DragonFire
														// configuration
														// Activity.
	public final static String TEST_MODE_START_LOGSERVICE = "00002007";
	
	private final static String FLAG_USBHOST = "/mnt/usbhost1/DragonFire/custom_cases.xml";
	private final static String FLAG_USBHOST_CONFIG = "/mnt/usbhost1/DragonFire/";
	private final static String FLAG_EXTSD = "/mnt/extsd/DragonFire/custom_cases.xml";
	private final static String FLAG_EXTSD_CONFIG = "/mnt/extsd/DragonFire/";
	private final static String FLAG_SDCARD = "/mnt/sdcard/DragonFire/custom_cases.xml";
	private final static String FLAG_SDCARD_CONFIG = "/mnt/sdcard/DragonFire/";
}

