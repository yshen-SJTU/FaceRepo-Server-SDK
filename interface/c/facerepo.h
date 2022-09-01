#ifndef __FACE_REPO__
#define __FACE_REPO__

//init and uninit in C the simple interface
void repo_init(std::string repo_filename, size_t max_elements=100000);
void repo_uninit();

//load a repo bin file
int repo_load(std::string repo_filename, int max_elements);
int repo_load(std::string repo_filename);

//search
std::vector<std::pair<size_t, float>> repo_search(const float *face_feature, float thres_sim=0.8, size_t max=100);

//add files or dir to repo
int repo_addImage(std::string path, std::vector<std::pair<size_t, std::string>> *faceIDs);
//add face to repo
size_t repo_addFace(const float *face_feature);

//remove face from repo
int repo_removeFace(size_t face_id);
//remove faces from repo
int repo_removeFaces(size_t *face_ids, int num);

//resize the current repo
int repo_resize(int max_elements);

#endif
