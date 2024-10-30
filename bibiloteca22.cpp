#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
using namespace std;

struct Livro {
    char codigo[10], titulo[100], autor[100], editora[100], area[50], dtEmprestimo[11], dtDevolucao[11], usuario[50];
    int paginas;
};

vector<Livro> biblioteca;

void salvarLivros() {
    FILE *file = fopen("biblioteca.dat", "w");
    if (!file) return;
    for (const auto &livro : biblioteca)
        fprintf(file, "%s;%s;%s;%s;%s;%d;%s;%s;%s\n",
                livro.codigo, livro.titulo, livro.autor, livro.editora,
                livro.area, livro.paginas, livro.dtEmprestimo, livro.dtDevolucao, livro.usuario);
    fclose(file);
}

void carregarLivros() {
    FILE *file = fopen("biblioteca.dat", "r");
    if (!file) return;
    Livro livro;
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^;];%[^;];%[^\n]\n",
                  livro.codigo, livro.titulo, livro.autor, livro.editora,
                  livro.area, &livro.paginas, livro.dtEmprestimo, livro.dtDevolucao, livro.usuario) == 9) {
        biblioteca.push_back(livro);
    }
    fclose(file);
}

Livro* encontrarLivro(const char *codigo) {
    for (auto &livro : biblioteca) {
        if (!strcmp(livro.codigo, codigo)) return &livro;
    }
    return nullptr;
}

void cadastrarLivro() {
    Livro livro = {};
    cout << "Código: ";
    cin >> livro.codigo;
    cout << "Título: ";
    cin.ignore();
    cin.getline(livro.titulo, 100);
    cout << "Autor: ";
    cin.getline(livro.autor, 100);
    cout << "Editora: ";
    cin.getline(livro.editora, 100);
    cout << "Área: ";
    cin.getline(livro.area, 50);
    cout << "Páginas: ";
    cin >> livro.paginas;

    biblioteca.push_back(livro);
    salvarLivros();
}

void alterarLivro() {
    char codigo[10];
    cout << "Código do livro: ";
    cin >> codigo;

    if (Livro *livro = encontrarLivro(codigo)) {
        cout << "Novo título: ";
        cin.ignore();
        cin.getline(livro->titulo, 100);
        cout << "Novo autor: ";
        cin.getline(livro->autor, 100);
        cout << "Nova editora: ";
        cin.getline(livro->editora, 100);
        cout << "Nova área: ";
        cin.getline(livro->area, 50);
        cout << "Novo nº de páginas: ";
        cin >> livro->paginas;

        salvarLivros();
    }
}

void excluirLivro() {
    char codigo[10];
    cout << "Código do livro: ";
    cin >> codigo;

    for (auto it = biblioteca.begin(); it != biblioteca.end(); ++it) {
        if (!strcmp(it->codigo, codigo)) {
            biblioteca.erase(it);
            salvarLivros();
            cout << "Livro excluído com sucesso." << endl;
            return;
        }
    }
}

void emprestarLivro() {
    char codigo[10], usuario[50], dtEmprestimo[11], dtDevolucao[11];
    cout << "Código do livro: ";
    cin >> codigo;

    if (Livro *livro = encontrarLivro(codigo)) {
        cout << "Data do empréstimo: ";
        cin >> dtEmprestimo;
        cout << "Data da devolução: ";
        cin >> dtDevolucao;
        cout << "Usuário: ";
        cin.ignore();
        cin.getline(usuario, 50);

        strcpy(livro->dtEmprestimo, dtEmprestimo);
        strcpy(livro->dtDevolucao, dtDevolucao);
        strcpy(livro->usuario, usuario);

        salvarLivros();
    }
}

void devolverLivro() {
    char codigo[10];
    cout << "Código do livro: ";
    cin >> codigo;

    if (Livro *livro = encontrarLivro(codigo)) {
        strcpy(livro->dtEmprestimo, "");
        strcpy(livro->dtDevolucao, "");
        strcpy(livro->usuario, "");
        
        salvarLivros();
        cout << "Livro devolvido com sucesso." << endl;
    }
}

void consultarLivro() {
    char codigo[10];
    cout << "Código do livro: ";
    cin >> codigo;

    if (Livro *livro = encontrarLivro(codigo)) {
        cout << "Título: " << livro->titulo
             << "\nAutor: " << livro->autor
             << "\nEditora: " << livro->editora
             << "\nÁrea: " << livro->area
             << "\nPáginas: " << livro->paginas
             << "\nEmpréstimo: " << livro->dtEmprestimo
             << "\nDevolução: " << livro->dtDevolucao
             << "\nUsuário: " << livro->usuario << endl;
    }
}

void listarLivrosDisponiveis() {
    for (const auto &livro : biblioteca) {
        if (!strlen(livro.dtEmprestimo)) {
            cout << "Código: " << livro.codigo << "\nTítulo: " << livro.titulo << endl;
        }
    }
}

void mostrarMenu() {
    cout << "\n1 – Cadastro\n2 – Empréstimo\n3 – Devolução\n4 – Consulta\n5 – Livros disponíveis\n6 – Alteração\n7 – Exclusão\n8 – Sair\n";
}

int main() {
    carregarLivros();
    int opcao;

    do {
        mostrarMenu();
        cout << "Escolha: ";
        cin >> opcao;

        switch (opcao) {
            case 1: cadastrarLivro(); break;
            case 2: emprestarLivro(); break;
            case 3: devolverLivro(); break;
            case 4: consultarLivro(); break;
            case 5: listarLivrosDisponiveis(); break;
            case 6: alterarLivro(); break;
            case 7: excluirLivro(); break;
            case 8: cout << "Saindo...\n"; break;
            default: cout << "Opção inválida.\n";
        }
    } while (opcao != 8);

    return 0;
}