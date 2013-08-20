#ifndef NGPP_GRID_COMPONENT_H
#define NGPP_GRID_COMPONENT_H

#include "../math/math.h"
#include "../core/Component.h"
#include "../core/Entity.h"

#include "PositionComponent.h"

#include <vector>
#include <tuple>

namespace NGPP {
    namespace Components {
        using namespace std;

        class GridComponent : public Component {
        public:
            GridComponent(int width, int height) :
                Component(),
                width(width),
                height(height),
                positions(vector<PositionComponent*>(width * height))
            {
                for (int x = 0; x < width; x++) {
                    for (int y = 0; y < height; y++) {
                        auto posComp = new PositionComponent();

                        this->positions[this->_coordsToIndex(x, y)] = posComp;
                        posComp->SetPosition(x, y, 0.0f);
                    }
                }
            }

            void Swap(int index1, int index2)
            {
                auto posComp1 = this->positions[index1];
                auto posComp2 = this->positions[index2];

                float x1 = posComp1->GetX();
                float y1 = posComp1->GetY();
                float x2 = posComp2->GetX();
                float y2 = posComp2->GetY();

                posComp1->SetPosition(x2, y2, 0.0f);
                posComp2->SetPosition(x1, y1, 0.0f);

                this->positions[index1] = posComp2;
                this->positions[index2] = posComp1;
            }

            void Swap(int x1, int y1, int x2, int y2)
            {
                int index1 = this->_coordsToIndex(x1, y1);
                int index2 = this->_coordsToIndex(x2, y2);

                this->Swap(index1, index2);
            }

            PositionComponent *GetComponentAt(int x, int y)
            {
                int index = this->_coordsToIndex(x, y);

                return this->GetComponentAt(index);
            }

            PositionComponent *GetComponentAt(int index)
            {
                return index < this->positions.size() && index >= 0 ?
                    this->positions[index] : NULL;
            }

            int width;
            int height;

            vector<PositionComponent*> positions;

        private:
            int _coordsToIndex(int x, int y)
            {
                // Move down width * y number of rows, then add x
                return (this->width * y) + x;
            }

            tuple<int, int> _indexToCoords(int index) {
                return make_tuple(index % this->width, index % this->height);
            }
        };
    }
}

#endif //NGPP_GRID_COMPONENT_H