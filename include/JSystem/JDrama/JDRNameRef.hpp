#ifndef JDR_NAME_REF_HPP
#define JDR_NAME_REF_HPP

#include <types.h>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

namespace JDrama {

/**
 * @brief Base class for all scene graph objects referenceable by name.
 *
 * @details Each scene graph class derived from TNameRef has a unique-ish
 * string name that can be used in JDrama to create an instance of that type,
 * look up an existing instance (globally or nested within a container graph
 * node), or load an instance from a stream.
 */
class TNameRef {
public:
	TNameRef(const char* name)
	    : mName(name)
	    , mKeyCode(calcKeyCode(name))
	{
	}

	/// Hashes @p string to a 16-bit key code for faster comparisons
	static u16 calcKeyCode(char const* string);

	/**
	 * @brief Load the type of a nameref from a stream and return the
	 * remainder of the node's type-dependent data as a new stream.
	 *
	 * @param stream the stream to load from
	 * @param remainder stream with the remainder of the node's data
	 * @return the name of the node's type
	 */
	static const char* getType(JSUMemoryInputStream& stream,
	                           JSUMemoryInputStream& remainder);

	/**
	 * @brief Generates a new nameref of the type
	 * loaded from @p stream.
	 *
	 * @warning a freshly-generated nameref has not yet loaded it's
	 * type-dependent data from the stream, TNameRef::load must be called
	 * afterwards with the remainder stream to finish loading the object.
	 *
	 * @details Global TNameRefGen instance is used to look up the type
	 * by name and create a new instance.
	 *
	 * @param stream stream to load the type from
	 * @param remainder stream with the remainder of the node's data
	 * @return pointer to the new object
	 */
	static TNameRef* genObject(JSUMemoryInputStream& stream,
	                           JSUMemoryInputStream& remainder);

	virtual ~TNameRef();
	virtual int getType() const;

	/**
	 * @brief Loads the type-dependent data of the nameref from a stream.
	 * @details override this in derived classes to load various fields of
	 * instances of that class.
	 * @warning overrides of this function **must not** look up namerefs by
	 * name, as not all other objects are loaded yet. Use TNameRef::loadAfter
	 * for that.
	 *
	 * @param stream the stream to load from
	 */
	virtual void load(JSUMemoryInputStream& stream);
	/// @deprecated retail sms does not use this method
	virtual void save(JSUMemoryOutputStream& stream);
	/**
	 * @brief Does post-load initialization that requires all other objects
	 * to be loaded first, mostly looking up other objects by name.
	 */
	virtual void loadAfter();

	// TODO: which one of this was public API? is the inline even real?
	virtual TNameRef* searchF(u16 key, char const* name);
	TNameRef* search(const char* name)
	{
		return searchF(calcKeyCode(name), name);
	}

	// fabricated
	const char* getName() const { return mName; }

protected:
	/* 0x4 */ const char* mName;
	/* 0x8 */ u16 mKeyCode;
};

} // namespace JDrama

#endif
