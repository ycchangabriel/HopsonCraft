#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include <SFML/System/Clock.hpp>


#include "../Physics/AABB.h"

#include "Chunk/CSection.h"
#include "Chunk/CMap.h"
#include "World_Settings.h"

#include "Generators/World_Generator.h"

class Player;
class Camera;

namespace Renderer
{
    class Master;
}

class World
{
    enum class State
    {
        Nothing,
        Regenerating,
        Triggering,
    } m_state = State::Nothing;


    struct Position_Block
    {
        Position_Block(CBlock newBlock, const Vector3& blockPosition)
        :   block       (newBlock)
        ,   position    (blockPosition)
        { }

        CBlock block;
        Vector3 position;
    };

    public:
        World(const World_Settings& settings, const Camera& camera);
        ~World();

        void updateChunks(const Player& player);

        void checkPlayerBounds(Player& player);
        void drawWorld(Renderer::Master& renderer, const Camera& camera);

        void qSetBlock  (const Vector3& position, CBlock block);
        void setBlock   (const Vector3& position, CBlock block);
        CBlock getBlock (const Vector3& position);

        uint32_t getHeightAt (const Vector3& worldPosition);

        const World_Settings& getWorldSettings() const;

        AABB getBlockAABB(const Block::Position& position);

    private:
        void regenerateChunks   ();
        void triggerBlocks      ();
        void generateWorld      (const Camera& camera);


        void draw               (Renderer::Master& renderer, const Camera& camera);

        void checkChunksForDelete();

        World_Generator m_worldGen;

        std::vector<Position_Block> m_newBlocks;
        std::vector<Position_Block> m_triggerBlocks;
        std::vector<Position_Block> m_sheduledTriggerBlocks;

        std::vector<std::thread>    m_threads;
        std::vector<Chunk::Position>    m_deleteChunks;
        std::mutex m_deleteMutex;

        World_Settings  m_worldSettings;
        Chunk::Map      m_chunks;

        Chunk::Position m_cameraPosition;

        int m_loadingDistance = 1;
        bool m_isRunning = true;

        const Camera* m_pCamera = nullptr;

    public:
        ///@TODO Make this private somehow.
        ///I say "somehow", as it has to be accessed by the HUD debug display
        float m_facesDrawn = 0;
};

#endif // WORLD_H_INCLUDED
