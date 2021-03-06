#ifndef CSECTION_H_INCLUDED
#define CSECTION_H_INCLUDED

#include <array>
#include <atomic>

#include "../World_Constants.h"

#include "CPosition.h"
#include "CNodes.h"
#include "CMesh_Builder.h"
#include "../Block/Block_Database.h"
#include "../../Physics/AABB.h"

#include "../../Util/Array3D.h"

namespace Chunk
{
    class Map;
    class Full_Chunk;

    class Section
    {
        struct State
        {
            State()
            :   made    (false)
            ,   buffered(false)
            { }

            std::atomic<bool> made;
            std::atomic<bool> buffered;
        };

        //This struct helps with optimizing the creation of the mesh
        struct Layer
        {
            int16_t opaqueCount = 0;

            Layer() { }

            void update(CBlock block)
            {
                block.getData().isOpaque ?
                    opaqueCount++ :
                    opaqueCount--;

                if (opaqueCount < 0)
                {
                    opaqueCount = 0;
                }
            }
        };

        const static Layer errorLayer;

        public:
            Section(const Chunklet_Position& position, Map& map, Full_Chunk& fullChunk);

            const State& getStates() const;

            void makeMesh   ();
            void bufferMesh ();

            void tick(World& world);

            //Blocks getters/ Setters
            //with bounds checking
            void    setBlock(Block::Small_Position& position, CBlock block);
            CBlock  getBlock(Block::Small_Position& position) const;
            //with no bounds checking
            void    qSetBlock(const Block::Small_Position& position, CBlock block);
            CBlock  qGetBlock(const Block::Small_Position& position) const;

            //Natural lights getters/ setters
            //with bounds checking
            void setNaturalLight(Block::Small_Position& position, uint8_t value);
            uint8_t getNaturalLight(Block::Small_Position& position) const;
            //with no bounds checking
            void qSetNaturalLight(const Block::Small_Position& position, uint8_t value);
            uint8_t qGetNaturalLight(const Block::Small_Position& position) const;

            //Block lights getters/ setters
            //with bounds checking
            void setBlockLight(Block::Small_Position& position, uint8_t value);
            uint8_t getBlockLight(Block::Small_Position& position) const;
            //with no bounds checking
            void qSetBlockLight(const Block::Small_Position& position, uint8_t value);
            uint8_t qGetBlockLight(const Block::Small_Position& position) const;

            const Chunklet_Position& getPosition()  const;
            const Meshes& getMeshes     ()          const;
            const Layer& getLayer       (int8_t y)  const;

            const Section* getAdjacentSection(const Vector2& change) const;
            const AABB& getAABB() const;
            const Full_Chunk& getParentChunk() const;

        private:
            bool m_hasUpdatableBlocks = false;

            State m_states;

            Full_Chunk& getParentChunk();

            Array3D<CBlock, CHUNK_SIZE>     m_blocks;
            Array3D<CLight, CHUNK_SIZE>     m_light;
            std::array<Layer, CHUNK_SIZE>   m_layerHasAir;

            Chunklet_Position m_position;

            Mesh_Builder m_meshBuilder;
            Meshes       m_meshes;

            Map* m_pChunkMap;
            Full_Chunk* m_parentChunk;

            AABB m_aabb;
    };
}

#endif // CSECTION_H_INCLUDED
