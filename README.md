# FaceRepo Server SDK
The SDK is used to develop a face repository server that can be deployed to a CPU server. A face repository is a graph in its data. Each node in the graph is a face's feature (vector), and the link between two nodes helps ANN query. The SDK is based on hnsw and provides C and Java APIs of repository management and KNN search for similar faces.


## Deployment
The SDK is for platform of Ubuntu server 20.04 or later. You can copy the library file "libxcaan_facerepo.so" in directory lib to /usr/lib64 or anywhere you like, and then config as following:

```bash
sudo vi /etc/ld.so.conf.d/facerepo.conf 
```

Add following two lines to faceengine.conf,
```bash
/usr/lib64     # the dir containing libxcaan_facerepo.so
```

and run
```bash
sudo ldconfig
```

The deployment should be OK. If your current server has FaceEngine Server SDK deployed, you can run following example to test FaceRepo.
```bash
cd examples/c
./build/main 
```

If you see the output like the following:
```bash
---- Test 1
Image processing:
Result faces:
    (0) fid: 225186875533561856  image: ./images/jiang.jpeg
    (1) fid: 225186875533561857  image: ./images/test.png
    (2) fid: 225186875533561858  image: ./images/test.png
    (3) fid: 225186875533561859  image: ./images/deng.jpeg

---- Test 2
Get face from picture: ./deng2.jpeg
Result: 
    get 1 face(s)
Search similar faces of the face from repo ...
Search result:
    (0) fid: 225186875533561859  similarity: 0.842132
...
```

then congratulations! The environment is OK for development.

## Using FaceRepo server SDK

FaceRepo server SDK provides both C and Java APIs.

### Develop applications using C APIs

A simple C example:

```C
#include "faceengine.h"
#include "facerepo.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
int main(int argc, char* argv[]) {

    init_engine(true); //initialize faceengine since repo_addImage will scan images to extract face features 

    cout << "---- Test 1" << endl;

    repo_init("./test.bin", 1000); //init a repo and save to file ./test.bin

    vector<pair<size_t, string>> faceIDs;
    repo_addImage("./images", &faceIDs); //scan ./images to get face features, and save the features to the repo. Each face is assigned an ID.
    cout <<"Result faces:" << endl;
    if(faceIDs.size() > 0) {
        for(int i=0; i<faceIDs.size(); i++) {
            cout << "    (" << i << ") fid: " << faceIDs[i].first << "  image: " << faceIDs[i].second << endl;
        }
    }

    //2. load and search
    cout << "\n---- Test 2" << endl;

    repo_load("./test.bin"); //load the repo file saved above

    vector<FaceInfo_t> infos = getFaceFeature("./deng2.jpeg"); //get face feature from ./deng2.jpeg

    if(infos.size() > 0) {
        
        vector<pair<size_t, float>> result = repo_search(infos[0].feature.data(), 0.8, 10); //search the repo to find similar faces with threshold 0.8
        
        for(int k=0; k<result.size(); k++) {
            cout << "    (" << k << ") fid: " << result[k].first << "  similarity: " << result[k].second << endl;
        }
    }

    repo_uninit();

    ...
    
}
```

The example in directory c including CMakeLists.txt can be the start of your application development.

<p id="CAPI"></p>

#### The C APIs of FaceRepo server SDK

1. Init/Uninit function
```C
void repo_init(std::string repo_filename, size_t max_elements=100000);
```

Initialize memory for the face repository. Face information added later will be saved to file indicated by repo_filename. If there is a file with the same name, the original file will be overwritten. 

Parameters
 - repo_filename - full path filename of the face repository.
 - max_elements - the maximal amount of faces the repository can contain. The repository capacity can be adjusted later using repo_resize.


```C
void repo_uninit();
```

Release memory for the face repository. You must initialize again or load a repository file before you can invoke other APIs. 

 
2. Load a repository
```C
int repo_load(std::string repo_filename, int max_elements);
int repo_load(std::string repo_filename);
```

Load a repository file into memory. It will initialize memory first and then load data from repository file.

Parameters
 - repo_filename - full path filename of the face repository.
 - max_elements - the maximal amount of faces the repository can contain.

Return
 - 0 if succeed, otherwise fail.

3. Add a face feature to repository
```C
size_t repo_addFace(const float *face_feature);
```

Parameters
 - face_feature - a face feature. The data type is float array.

Return
 - face id, a unique number indicates the face in the repository.

4. Scan images and add faces to repository
```C
int repo_addImage(std::string path, std::vector<std::pair<size_t, std::string>> *faceIDs);
```

Parameters
 - path - full path directory that contains images. The subdirs will be recursively scanned.
 - faceIDs - a vector containing pairs of (face id, image filename). It is returned from the API. Any face id in the vector is unique indicating a unique face in the repository.

Return
 - amount of added faces if succeed. Otherwise a negative number if fail.


5. Remove face from repository
```C
int repo_removeFace(size_t face_id);
int repo_removeFaces(size_t *face_ids, int num);
```

Parameters
 - face_id - face id of the face that needs to be removed.
 - face_ids - an array of face ids.
 - num - amount of face ids in the array.

Return
 - 0 if succeed, otherwise fail.

6. Search
```C
std::vector<std::pair<size_t, float>> repo_search(const float *face_feature, float thres_sim=0.8, size_t max=100);
```

Parameters
 - face_feature - the feature of a REFERENCE FACE that is used to search similar faces in the repository.
 - thres_sim - similarity threshold that is in [0, 1]. We recommend a threshold in [0.8, 0.9] if using our FaceEngine.
 - max - maximal amount of faces returned by the search.

Return
 - vector of (face id, similarity) where the similarity is a cosine-based distance between the reference face and the face identified by face id.

7. Resize repository capacity
```C
int repo_resize(int max_elements);
```

Parameters
 - max_elements - the new capacity of the current repository.

Return
  - 0 if succeed, otherwise fail.


 
### Develop applications using Java APIs

A simple Java example:

```Java
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

    System.out.println("---- Test 1");

    frepo.init("./test.bin", 2000);

    Map<Long, String> faceIDs = new HashMap<>();
    frepo.add(dir+"/images", faceIDs, new TestFaceRepo()); //scan image directory and add all detected faces to the repository

    Iterator iter = faceIDs.entrySet().iterator();
    int i=0;
    while (iter.hasNext()) {
        ++i;
        Map.Entry entry = (Map.Entry) iter.next();
        System.out.println("    ("+i+") fid: "+entry.getKey().toString()+"  image: "+entry.getValue().toString());
    }

    System.out.println("\n---- Test 2");

    frepo.load("./test.bin", 3000); //load a repo file

    List<FaceInfo> faces = FaceEngine.getFaceFeature(pic); //get a face's feature
	
    if(faces.size() > 0) {
        Map<Long, Float> results = frepo.search(faces.get(0).feature, 0.8f, 10); //search
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

	...

    public int fileCount(int count){ //callback for face detection task
        System.out.println("    Remaining files:"+count);
        return count;
    }
}
```

#### The Java APIs of FaceRepo

The APIs provided by FaceRepo are quite simple. About their parameters and returns, please refer to ["The C APIs of FaceRepo server SDK"](#CAPI).



## License

The SDK can be used for any personal or commercial purpose. The SDK is distributed 'as is' and with no warranties of any kind, whether express or implied. The user must assume the entire risk of using the SDK, and the development team won't be held responsible for any problem in your using the SDK.


## The Development Team

FaceRepo is a cooperation between Shanghai Jiao Tong University and XCAAN with several engineers and students contributing to it.

Please contact <yshen@cs.sjtu.edu.cn> if you have problems about using the SDK. Any technical exchange and cooperation are welcomed.



