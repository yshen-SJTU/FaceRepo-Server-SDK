#include "faceengine.h"
#include "facerepo.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
int main(int argc, char* argv[]) {

    init_engine(true);

    //1. init and add faces from images
    cout << "---- Test 1" << endl;

    repo_init("./test.bin", 1000);

    cout << "Image processing:" << endl;

    vector<pair<size_t, string>> faceIDs;
    repo_addImage("./images", &faceIDs);
    cout <<"Result faces:" << endl;
    if(faceIDs.size() > 0) {
		for(int i=0; i<faceIDs.size(); i++) {
			cout << "    (" << i << ") fid: " << faceIDs[i].first << "  image: " << faceIDs[i].second << endl;
		}
    }

    //2. load and search
	cout << "\n---- Test 2" << endl;

    repo_load("./test.bin");

    cout << "Get face from picture: ./deng2.jpeg" << endl;

    vector<FaceInfo_t> infos = getFaceFeature("./deng2.jpeg");
    cout << "Result: " << endl;

    if(infos.size() > 0) 
        cout << "    get " << infos.size() << " face(s)"<< endl;
    else
        cout << "    get no face!" << endl;

    cout << "Search similar faces of the face from repo ..." << endl;
	cout << "Search result:" << endl;

    if(infos.size() > 0) {
        
        vector<pair<size_t, float>> result = repo_search(infos[0].feature.data(), 0.8, 10);
        if(result.size() == 0)
            cout << "    null!" << endl;
        else {
            for(int k=0; k<result.size(); k++) {
                cout << "    (" << k << ") fid: " << result[k].first << "  similarity: " << result[k].second << endl;
            }
        }
    }

    repo_uninit();

    //3. load and add face
    cout << "\n---- Test 3" << endl;

    repo_load("./test.bin", 3000);

    cout << "Add a face to the repo ..." << endl;
    size_t fid = repo_addFace(infos[0].feature.data());
    cout << "    The added face id is " << fid << endl;

    cout << "Again search similar faces of the face from repo ..." << endl;
    cout << "Search result:" << endl;
    
    vector<pair<size_t, float>> result = repo_search(infos[0].feature.data(), 0.8, 10);
    if(result.size() == 0)
        cout << "    null!" << endl;
    else {
        for(int k=0; k<result.size(); k++) {
            cout << "    (" << k << ") fid: " << result[k].first << "  similarity: " << result[k].second << endl;
        }
    }
}