#pragma once

namespace Engine
{
    template <class TYPE>
	class EntityFactory
	{
	public:
    protected:
        template<typename... Args>
        TYPE* CreateAndPushWaitQueue(Args&&... args)
        {
            //static_assert(std::is_base_of<EntityFactory, TYPE>::value, "Create_Fail");
            TYPE* instance = new TYPE(std::forward<Args>(args)...);
            mCreateQueue.push(instance);
            return instance;
        }
        void CreateUpdate()
        {
            while (!mCreateQueue.empty())
            {
                mCreateQueue.front()->SetActive(true);
                mCreateQueue.pop();
            }
        }
        void DestroyUpdate()
        {
            while (!mDestroyQueue.empty())
            {
                SAFE_DELETE(mDestroyQueue.front());
                mDestroyQueue.pop();
            }
        }

        TYPE* GetCreation()
        {
            if (mCreateQueue.empty())
            {
                return nullptr;
            }
            else
            {
                TYPE* pEntity = mCreateQueue.front();
                mCreateQueue.pop();
                pEntity->SetActive(TRUE);
                return pEntity;
            }
        }
    protected:
        std::queue<TYPE*> mCreateQueue;
        std::queue<TYPE*> mDestroyQueue;
	};
}


