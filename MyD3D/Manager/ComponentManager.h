

class ComponentManager
{
public:
	bool Initialize();
	void Run();
	void Finalization();

	ComponentFactory factory;
};