
#include <iostream>
#include <algorithm>
#include "ipc_rewriter.h"
#include "../modules/ipc_module.h"

#include <boost/filesystem.hpp>

using namespace boost;

using namespace std;

namespace ipcrewriter {

	void IpcRewriter::addModule(std::shared_ptr<IpcModule> module) {
		modules.push_back(module);
	}

	void IpcRewriter::createIpcJail(const ContextConfig *contextConfig, const YamlConfig* config) {
		for (const auto &m : modules) {
			cerr << "Loading " << m->getSource() << "..." << endl;
			m->loadModuleFile();
			/*cerr << "Imports:" << endl;
			for (const auto &s: m->getImports()){
				cerr << "- " << s << endl;
			}
			cerr << "Exports:" << endl;
			for (const auto &s: m->getExports()){
				cerr << "- " << s << endl;
			}*/
		}

		// that's it for now
		createCommunicationPair(*modules[0], *modules[1], contextConfig, config);
	}

	void IpcRewriter::createCommunicationPair(IpcModule &m1, IpcModule &m2, const ContextConfig *contextConfig, const YamlConfig *config) {
		auto cmp = make_shared<CommunicationPair>(&m1, &m2);
		if (contextConfig) {
			cmp->setNsjailConfig(&contextConfig->permissions);
		}
		if (config) {
			//if (!config->filedescriptors.empty())
			cmp->setFileDescriptorPolicy(&config->filedescriptors);
		}
		if (m1.canRewrite()) {
			set<string> rewriting;
			set_intersection(m2.getExports().begin(), m2.getExports().end(), m1.getImports().begin(), m1.getImports().end(),
							 inserter(rewriting, rewriting.begin()));
			m1.rewriteFunctionCalls(*cmp, rewriting);
		}
		communications.push_back(cmp);
	}

	void IpcRewriter::saveAllModules(const string &basepath) {
		for (const auto &m : modules) {
			if (m->canRewrite()) m->save();
		}

		new_files.clear();
		for (auto &c : communications) {
			auto rnd = std::rand();
			auto fname = filesystem::path(basepath).append("communication-" + to_string(c->getId()) + "-" + to_string(rnd) + ".bc");
			c->save(fname.string());
			new_files.push_back(fname.string());
		}
	}

}
