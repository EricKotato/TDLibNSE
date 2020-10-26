#pragma once
#include <td/telegram/td_json_client.h>
#include "sciter-x.h"

namespace tdlibnse {

class TDLib : public sciter::om::asset<TDLib> {
public:
	TDLib() {}

	int createClient() {
		return td_create_client();
	}

	sciter::value send(int client_id, sciter::string request) {
		td_send(client_id, aux::w2a(request.c_str()).c_str());
		return sciter::value();
	}

	sciter::string receive(double timeout) {
		const auto chars = aux::utf2w(td_receive(timeout));
		return sciter::string(chars.c_str(), chars.length());
	}

	sciter::string execute(sciter::string request) {
		const auto chars = aux::utf2w(td_execute(
			aux::w2a(request.c_str()).c_str()
		));
		return sciter::string(chars.c_str(), chars.length());
	}

	SOM_PASSPORT_BEGIN(TDLib)
		SOM_PASSPORT_FLAGS(SOM_EXTENDABLE_OBJECT)
		SOM_FUNCS(
			SOM_FUNC(createClient),
			SOM_FUNC(send),
			SOM_FUNC(receive),
			SOM_FUNC(execute)
		)
	SOM_PASSPORT_END
};

} // namespace tdlibnse