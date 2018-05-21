#include "../DefQuery/enumerator.h"

template<typename TValue, typename TDerived>
class generator_enumerator : public enumerator<TValue, TDerived>
{
public:
	bool operator++() override
	{
		auto notExhausted = try_calculate_next(_current);
		return notExhausted;
	}

	const value_type& operator*() const override { return _current; }

protected:
	virtual bool try_calculate_next(TValue& nextValue) = 0;

private:
	TValue _current;
};

