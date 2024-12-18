const { ethers } = require('ethers');

// Telos EVM 测试网的RPC URL
const rpcUrl = 'https://testnet.telos.net/evm';

// 创建一个新的Provider
const provider = new ethers.JsonRpcProvider(rpcUrl);

// 使用你的私钥
const privateKey = '24a4c99b2d284ffe06ba0f9ead220d3aadf468eceef44bb42001a8db31b32e56';
const wallet = new ethers.Wallet(privateKey, provider);

async function getBalance(address) {
    const balance = await provider.getBalance(address);
    console.log(`Balance of ${address}: ${ethers.formatEther(balance)} TLOS`);
}

const abi = ['合约ABI'];

async function callContractMethod(contractAddress) {
    // create contract instance
    const contract = new ethers.Contract(contractAddress, abi, wallet);

    // call method
    const tx = await contract.setNumber(1);
    console.log(`https://testnet.teloscan.io/tx/${tx.hash}`);
    const result = await contract.number();
    console.log('number result:', result);
}