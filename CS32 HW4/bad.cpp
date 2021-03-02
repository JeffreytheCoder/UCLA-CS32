void removeBad(list<Movie*>& li)
{
	list<Movie*>::iterator it = li.begin();
	while (it != li.end()) {
		if ((*it)->rating() < 55) {
			delete (*it);
			it = li.erase(it);  // rea
		}
		else
			it++;
	}
}