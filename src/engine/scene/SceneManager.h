#ifndef A_DARK_DISCOMFORT_SCENEMANAGER_H
#define A_DARK_DISCOMFORT_SCENEMANAGER_H

class SceneManager
{
public:

    // Define constants
    static constexpr const char DEFAULT_BASEDIR[] = "assets/scenes/";

    // Public methods

    /**
     * Closes the current scene and starts a new, empty scene
     */
    static void NewScene();

    /**
     * Saves the currently loaded scene by its name, or "untitled"
     * if the scene is not already saved
     */
    static void SaveScene();

    /**
     * Saves the currently loaded scene by the name supplied
     * @param sceneName - the name to save the scene as
     */
    static void SaveScene(const std::string& sceneName, const std::string& baseDir = DEFAULT_BASEDIR);

    /**
     * Safely prepares the current scene for loading of a newly
     * specified scene
     * @param sceneName - the name of the new scene to load
     */
    static void QueueNextScene(const std::string& sceneName);

    /**
     * Loads a newly queued scene if one is set, should be called
     * at the end of the frame
     */
    static void LoadNextScene(const std::string& baseDir = DEFAULT_BASEDIR);

private:

    // Private methods

    static inline std::string MakeScenePath(const std::string& sceneName, const std::string& baseDir)
    {
        return baseDir + sceneName + ".scene";
    }

    // Private methods

    /**
     * Clears all entities in the scene (and related resources)
     */
    static void ClearScene();

    // Private fields

    /**
     * The name of the currently loaded scene
     */
    static std::string currentScene;

    /**
     * The name of the next scene to be loaded (if any)
     */
    static std::string nextScene;
};

#endif //A_DARK_DISCOMFORT_SCENEMANAGER_H
