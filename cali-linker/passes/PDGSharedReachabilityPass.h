#ifndef LLVMREWRITER_PDGSHAREDREACHABILITYPASS_H
#define LLVMREWRITER_PDGSHAREDREACHABILITYPASS_H

#include "PDG.h"
#include "DataDrivenSCCPass.h"
#include "../cali_linker/configfile.h"

class PDGSharedReachabilityPass : public llvm::ModulePass {
	PDG *pdg = nullptr;
	DataDrivenSCCPass *sccpass = nullptr;
	const YamlConfig *config;
	const ContextConfig *contextConfig;

public:
	static char ID;

	PDGSharedReachabilityPass() : llvm::ModulePass(ID), config(nullptr), contextConfig(nullptr) {}

	explicit PDGSharedReachabilityPass(const YamlConfig *config, const ContextConfig *contextConfig)
		: llvm::ModulePass(ID), config(config), contextConfig(contextConfig) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;


protected:

	template<class T>
	bool propagateCallInfluence(T& call, bool isCalleeToCaller);

	void matchSubnodes(std::map<Vertex, Vertex> &map, Vertex key, Vertex value);

	void propagateInfluenceToCallers(const std::vector<llvm::Function*>& functions);

	bool propagateInfluenceFromCallers(const std::vector<llvm::Function*>& functions);

	void warnForPossiblyDangerousActions(llvm::Module &M);

	bool considerFunction(const llvm::Function* f);
};

#endif //LLVMREWRITER_PDGSHAREDREACHABILITYPASS_H
