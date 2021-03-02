void listAll(const File* f, string path)  // two-parameter overload
{
    if (f->files() == nullptr) {
        cout << path + "/" + f->name() << endl;
        return;
    }
    path += ("/" + f->name());
    cout << path << endl;
    for (File* subFile : *(f->files())) {
        listAll(subFile, path);
    }
}