// Pretty print for Connected Componets
{
int i = 0;
std::cout << "======================================" << std::endl;
std::cout << "vid: " + std::__cxx11::to_string(vid)  +
			 "  -  Comp #: " + std::__cxx11::to_string(components.size()) +
			 "  -  Labels #: " + std::__cxx11::to_string(labels.size()) << std::endl;

for (auto comp : components){
	std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
	for(auto pid : comp){
		std::cout << std::__cxx11::to_string(pid) + "\t -> Label: " + std::__cxx11::to_string(m.poly_data(pid).label) << std::endl;
	}
	++i;
}
// END Pretty print
}