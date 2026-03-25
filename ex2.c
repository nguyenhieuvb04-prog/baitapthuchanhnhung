#include <stdio.h>
#include <stdlib.h>

// 1. Định nghĩa Bitfield để tiết kiệm bộ nhớ cho các cờ trạng thái (Flags)
struct Status {
    unsigned int isActive : 1;  // 1 bit: 0 hoặc 1
    unsigned int errorLog : 1;  // 1 bit
    unsigned int priority : 3;  // 3 bit: giá trị từ 0-7
};

// 2. Định nghĩa Union để lưu trữ dữ liệu đa thức (Số nguyên hoặc Số thực)
union Value {
    int i_data;
    float f_data;
};

// 3. Cấu trúc Node cho Cây Nhị Phân (Binary Tree) kết hợp các kiểu dữ liệu trên
struct tnode {
    int id;                     // Khóa của cây
    struct Status config;       // Bitfield
    union Value val;            // Union
    struct tnode *left;
    struct tnode *right;
}

// (a) Hàm cấp phát bộ nhớ động (Memory Allocation) cho một nút mới
struct tnode* talloc(int id, int prio, float data) {
    struct tnode* p = (struct tnode*)malloc(sizeof(struct tnode));
    if (p != NULL) {
        p->id = id;
        p->config.isActive = 1;      // Kích hoạt mặc định
        p->config.errorLog = 0;      // Không có lỗi
        p->config.priority = prio;   // Gán độ ưu tiên (0-7)
        p->val.f_data = data;        // Gán dữ liệu kiểu float
        p->left = p->right = NULL;
    }
    return p;
}

// (b) Hàm thêm nút vào cây (Binary Search Tree)
struct tnode* addnode(struct tnode* p, int id, int prio, float data) {
    if (p == NULL) {
        return talloc(id, prio, data);
    }
    if (id < p->id) {
        p->left = addnode(p->left, id, prio, data);
    } else if (id > p->id) {
        p->right = addnode(p->right, id, prio, data);
    }
    return p;
}

// (d) Duyệt cây theo thứ tự In-order (Dữ liệu sẽ được sắp xếp theo ID)
void inorder(struct tnode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("[ID: %d | Prio: %d | Val: %.2f]\n", 
                root->id, root->config.priority, root->val.f_data);
        inorder(root->right);
    }
}

// (e) Hàm xóa cây và giải phóng bộ nhớ (Dùng Post-order traversal)
int deltree(struct tnode* root) {
    if (root == NULL) return 0;
    int count = deltree(root->left) + deltree(root->right);
    free(root); // Giải phóng bộ nhớ động
    return count + 1;
}

// (f) Mã kiểm thử (Test Code)
int main() {
    struct tnode* root = NULL;

    printf("--- KHOI TAO HE THONG CAM BIEN (BST) ---\n");
    // Thêm các nút với ID, Độ ưu tiên (0-7), và Giá trị cảm biến
    root = addnode(root, 50, 2, 25.5);
    root = addnode(root, 30, 5, 12.8);
    root = addnode(root, 70, 1, 45.2);
    root = addnode(root, 20, 7, 10.0);
    root = addnode(root, 40, 3, 33.3);

    printf("\nDanh sach cam bien (Sap xep theo ID):\n");
    inorder(root);

    // Kiểm tra kích thước của Structure (để thấy hiệu quả của Bitfield/Union)
    printf("\nKich thuoc cua mot Node: %zu bytes\n", sizeof(struct tnode));

    printf("\nDang giai phong bo nho...\n");
    int deleted = deltree(root);
    printf("Da xoa thanh cong %d nut. He thong sach.\n", deleted);

    return 0;
}