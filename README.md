# Tugas Besar 1 IF3130 - Jaringan Komputer

## Sliding Window

### Petunjuk penggunaan program
* masuk ke directory
* jalankan perintah
```
make
```
* jalankan receive file dengan perintah dan parameter sebagai berikut:
```
./recvfile <filename> <windowsize> <buffersize> <port>
```
* lalu jalankan send file dengan perintah dan parameter sebagai berikut:
```
./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>
```

### Cara kerja sliding window pada program
Hal pertama yang dilakukan yaitu, sender mengirimkan data. Setelah itu receiver akan menerima data beserta sequence numbernya. Jika data berhasil diterima, receiver akan menyimpan nilai sequence number terbesar yang semua data sebelumnya sudah diterima. Receiver kemudian akan mengirimkan sequence number terbesar tersebut kepada sender. Jika ACK tersebut diterima oleh sender, maka nilai sequence number akan bergeser sesuai nilai yang diterima dari ACK. Sender kemudian akan mengirimkan kembali data terkecil dari sequence number hingga semua data diterima oleh receiver dan sender juga menerima ACK data terakhir.

### Pembagian tugas
Tugas dibagi sesuai kemampuan, usahan, dan do'a

### Jawaban pertanyaan
**1.** Sender harus menunggu receiver mengirimkan ACK yang menyatakan advertised window lebih dari 0, untuk bisa melanjutkan pengiriman paket

**2.** (besar ukuran header dalam bit)

+----+----+--------+--------+----+------+-------+------+--------+----+-----+
| SP | DP | Seqnum | ACKnum | DO | Resv | Flags | Wndw | Chksum | UP | Opt |
| 16 | 16 |   32   |   32   | 4  |   6  |   6   |  16  |   16   | 16 | 32  |
+----+----+--------+--------+----+------+-------+------+--------+----+-----+

* **Source Port (SP)** : merupakan identitas pengirim paket untuk aplikasi tertentu
* **Destination Port (DP)** : merupakan indentitas alamat kemana paket harus dikirimkan
* **Sequence number (Seqnum)** : urutan paket yang dikirim
* **Acknowledgment number (ACKnum)** : menendakan suatu pesan telah terkirim atau belum dan untuk mengindikasikan pesan mana yang selanjutnya harus dikirim oleh pengirim
* **Data Offset (DO)** : mengindikasikan ukuran dari header TCP dan dimana data dalam segmen TCP dimulai
* **Reserved (Resv)** : disediakan atau dialokasikan untuk digunakan pada masa depan
* **Flags** : sebagai penanda pesan yang dikirim
* **Window (Wndw)** : berguna untuk membatasi berapa banyak data yang dikirim sebelum menerima ACK dari server
* **Checksum (Chksum)** : melakukan pengecekan untuk data yang dikirim, apakah pesan yang terkirim berubah selama pengiriman atau tidak.
* **Urgent Pointer (UP)** : menandakan lokasi data yang dianggap "urgent"
* **Options (Opt)** : penampung opsi tambahan TCP

### Author
**Sliding Tackle Group**
