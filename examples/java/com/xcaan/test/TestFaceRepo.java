package com.xcaan.test;

import java.util.*;

import com.xcaan.faceengine.*;
import com.xcaan.facerepo.*;

public class TestFaceRepo implements ICallback {
	public static void main(String[] args){

		String dir = "./com/xcaan/test";
		String pic = dir + "/deng2.jpeg";

		FaceEngine.init(true);

        FaceRepo frepo = new FaceRepo();

		//1. init and add faces from images
		System.out.println("---- Test 1");

		frepo.init("./test.bin", 2000);

		System.out.println("Image processing:");
		Map<Long, String> faceIDs = new HashMap<>();
		frepo.add(dir+"/images", faceIDs, new TestFaceRepo());

		System.out.println("Result faces:");
		Iterator iter = faceIDs.entrySet().iterator();
		int i=0;
		while (iter.hasNext()) {
			++i;
			Map.Entry entry = (Map.Entry) iter.next();
			System.out.println("    ("+i+") fid: "+entry.getKey().toString()+"  image: "+entry.getValue().toString());
		}

		//2. load and search
		System.out.println("\n---- Test 2");

		frepo.load("./test.bin", 3000);

		System.out.println("Get face from picture: " + pic);
		List<FaceInfo> faces = FaceEngine.getFaceFeature(pic);
		System.out.println("Result: \n    get " + faces.size() + " face(s)");

		System.out.println("Search similar faces of the face from repo ...");
		System.out.println("Search result:");
		if(faces.size() > 0) {
			Map<Long, Float> results = frepo.search(faces.get(0).feature, 0.8f, 10);
			if(results == null){
				System.out.println("    null!");
			} else {
				iter = results.entrySet().iterator();
				i=0;
				while (iter.hasNext()) {
					++i;
					Map.Entry entry = (Map.Entry) iter.next();
					System.out.println("    ("+i+") fid: "+entry.getKey().toString()+"  similarity: "+entry.getValue().toString());
				}
			}
		}

		frepo.uninit();

		//3. load and add face
		System.out.println("\n---- Test 3");

		FaceRepo frepo2 = new FaceRepo();

		frepo2.load("./test.bin", 3000);

		System.out.println("Add a face to the repo ...");
		long fid2 = frepo2.add(faces.get(0).feature);
		System.out.println("    The added face id is " + fid2);

		System.out.println("Again search similar faces of the face from repo ...");
		System.out.println("Search result:");
		Map<Long, Float> results = frepo2.search(faces.get(0).feature, 0.8f, 10);
		if(results == null){
			System.out.println("    null!");
		} else {
			iter = results.entrySet().iterator();
			i=0;
			while (iter.hasNext()) {
				++i;
				Map.Entry entry = (Map.Entry) iter.next();
				System.out.println("    ("+i+") fid: "+entry.getKey().toString()+"  similarity: "+entry.getValue().toString());
			}
		}
	}


	public int fileCount(int count){
		System.out.println("    Remaining files:"+count);
		return count;
	}
}