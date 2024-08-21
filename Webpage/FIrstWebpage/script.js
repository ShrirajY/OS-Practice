// Files class definition
class Files {
    constructor(name) {
        this.name = name;
        this.content = '';
        this.buffers = [];
        this.inode = null;
    }

    create(content = '') {
        this.content = content;
        this.assignBuffers();
    }

    Write(text) {
        this.content = text;
        this.assignBuffers();
    }

    getContent() {
        return this.content;
    }

    getBuffers() {
        return this.buffers;
    }

    copy(otherFile) {
        this.content = otherFile.getContent();
        this.assignBuffers();
    }

    link(otherFile) {
        this.buffers.push(otherFile);
    }

    setInode(inode) {
        this.inode = inode;
    }

    getInode() {
        return this.inode ? this.inode.inodeNo : 'No inode assigned';
    }

    // Assign buffers with chunks of 30 characters
    assignBuffers() {
        this.buffers = [];
        const chunkSize = 30;
        for (let i = 0; i < this.content.length; i += chunkSize) {
            this.buffers.push(this.content.slice(i, i + chunkSize));
        }
    }
}

// Example inode class
class Inode {
    constructor(inodeNo) {
        this.inodeNo = inodeNo;
    }
}

// Initialize file system object
const files = {};
let inodeCounter = 1; // Global counter for inode numbers

// Function to create a file
function createFile() {
    const fileName = document.getElementById('fileName').value;
    if (!fileName) {
        alert('Please enter a file name.');
        return;
    }
    files[fileName] = new Files(fileName);
    // Assign a unique inode
    files[fileName].setInode(new Inode(inodeCounter++));
    alert(`File "${fileName}" created with inode ${files[fileName].getInode()}.`);
    updateFileTable();
}

// Function to write text to a file
function writeToFile() {
    const fileName = document.getElementById('writeFileName').value;
    const text = document.getElementById('writeText').value;
    if (!fileName || !files[fileName]) {
        alert('File does not exist.');
        return;
    }
    files[fileName].Write(text);
    alert(`Text written to file "${fileName}".`);
}

// Function to copy a file
function copyFile() {
    const sourceFileName = document.getElementById('copyFileName').value;
    const targetFileName = document.getElementById('copyToFileName').value;
    if (!sourceFileName || !files[sourceFileName] || !targetFileName) {
        alert('Please enter valid file names.');
        return;
    }
    files[targetFileName] = new Files(targetFileName);
    files[targetFileName].copy(files[sourceFileName]);
    // Copy inode (if desired, otherwise skip)
    files[targetFileName].setInode(new Inode(inodeCounter++));
    alert(`File "${sourceFileName}" copied to "${targetFileName}".`);
    updateFileTable();
}

// Function to link a file
function linkFile() {
    const sourceFileName = document.getElementById('linkFileName').value;
    const targetFileName = document.getElementById('linkToFileName').value;
    if (!sourceFileName || !files[sourceFileName] || !targetFileName || !files[targetFileName]) {
        alert('Please enter valid file names.');
        return;
    }
    files[sourceFileName].link(files[targetFileName]);
    alert(`File "${sourceFileName}" linked to "${targetFileName}".`);
}

// Function to rename a file
function renameFile() {
    const oldFileName = document.getElementById('renameFileName').value;
    const newFileName = document.getElementById('renameToFileName').value;
    if (!oldFileName || !files[oldFileName] || !newFileName) {
        alert('Please enter valid file names.');
        return;
    }
    files[newFileName] = files[oldFileName];
    files[newFileName].name = newFileName;
    delete files[oldFileName];
    alert(`File "${oldFileName}" renamed to "${newFileName}".`);
    updateFileTable();
}

// Function to delete a file
function deleteFile() {
    const fileName = document.getElementById('deleteFileName').value;
    if (!fileName || !files[fileName]) {
        alert('File does not exist.');
        return;
    }
    delete files[fileName];
    alert(`File "${fileName}" deleted.`);
    updateFileTable();
}

// Function to move a file
function moveFile() {
    const oldFileName = document.getElementById('moveFileFrom').value;
    const newFileName = document.getElementById('moveFileTo').value;
    if (!oldFileName || !files[oldFileName] || !newFileName) {
        alert('Please enter valid file names.');
        return;
    }
    files[newFileName] = files[oldFileName];
    files[newFileName].name = newFileName;
    delete files[oldFileName];
    alert(`File "${oldFileName}" moved to "${newFileName}".`);
    updateFileTable();
}

// Function to update the file table
function updateFileTable() {
    const tableBody = document.getElementById('fileTable').getElementsByTagName('tbody')[0];
    tableBody.innerHTML = ''; // Clear existing rows

    for (const fileName in files) {
        const file = files[fileName];
        const row = document.createElement('tr');

        // File name cell
        const fileNameCell = document.createElement('td');
        fileNameCell.textContent = fileName;
        row.appendChild(fileNameCell);

        // Show Buffers button
        const showBuffersCell = document.createElement('td');
        const showBuffersBtn = document.createElement('button');
        showBuffersBtn.textContent = 'Show Buffers';
        showBuffersBtn.className = 'button';
        showBuffersBtn.onclick = () => {
            const buffers = file.getBuffers();
            alert(`Buffers for "${fileName}": ${buffers.join(', ')}`);
        };
        showBuffersCell.appendChild(showBuffersBtn);
        row.appendChild(showBuffersCell);

        // Show Content button
        const showContentCell = document.createElement('td');
        const showContentBtn = document.createElement('button');
        showContentBtn.textContent = 'Show Content';
        showContentBtn.className = 'button';
        showContentBtn.onclick = () => {
            alert(`Content of "${fileName}": ${file.getContent()}`);
        };
        showContentCell.appendChild(showContentBtn);
        row.appendChild(showContentCell);

        // Show Inode button
        const showInodeCell = document.createElement('td');
        const showInodeBtn = document.createElement('button');
        showInodeBtn.textContent = 'Show Inode';
        showInodeBtn.className = 'button';
        showInodeBtn.onclick = () => {
            alert(`Inode No: ${file.getInode()}`);
        };
        showInodeCell.appendChild(showInodeBtn);
        row.appendChild(showInodeCell);

        // Copy To input and button
        const copyToCell = document.createElement('td');
        const copyToInput = document.createElement('input');
        copyToInput.type = 'text';
        copyToInput.placeholder = 'Copy to...';
        copyToCell.appendChild(copyToInput);
        const copyToBtn = document.createElement('button');
        copyToBtn.textContent = 'Copy';
        copyToBtn.className = 'button';
        copyToBtn.onclick = () => {
            const targetFileName = copyToInput.value;
            if (targetFileName) {
                files[targetFileName] = new Files(targetFileName);
                files[targetFileName].copy(file);
                // Copy inode (if desired, otherwise skip)
                files[targetFileName].setInode(new Inode(inodeCounter++));
                alert(`File "${fileName}" copied to "${targetFileName}".`);
                updateFileTable();
            }
        };
        copyToCell.appendChild(copyToBtn);
        row.appendChild(copyToCell);

        // Link To input and button
        const linkToCell = document.createElement('td');
        const linkToInput = document.createElement('input');
        linkToInput.type = 'text';
        linkToInput.placeholder = 'Link to...';
        linkToCell.appendChild(linkToInput);
        const linkToBtn = document.createElement('button');
        linkToBtn.textContent = 'Link';
        linkToBtn.className = 'button';
        linkToBtn.onclick = () => {
            const targetFileName = linkToInput.value;
            if (targetFileName && files[targetFileName]) {
                file.link(files[targetFileName]);
                alert(`File "${fileName}" linked to "${targetFileName}".`);
            } else {
                alert('Invalid target file for linking.');
            }
        };
        linkToCell.appendChild(linkToBtn);
        row.appendChild(linkToCell);

        // Rename input and button
        const renameCell = document.createElement('td');
        const renameInput = document.createElement('input');
        renameInput.type = 'text';
        renameInput.placeholder = 'Rename to...';
        renameCell.appendChild(renameInput);
        const renameBtn = document.createElement('button');
        renameBtn.textContent = 'Rename';
        renameBtn.className = 'button';
        renameBtn.onclick = () => {
            const newFileName = renameInput.value;
            if (newFileName) {
                files[newFileName] = files[fileName];
                files[newFileName].name = newFileName;
                delete files[fileName];
                alert(`File "${fileName}" renamed to "${newFileName}".`);
                updateFileTable();
            }
        };
        renameCell.appendChild(renameBtn);
        row.appendChild(renameCell);

        // Delete button
        const deleteCell = document.createElement('td');
        const deleteBtn = document.createElement('button');
        deleteBtn.textContent = 'Delete';
        deleteBtn.className = 'button';
        deleteBtn.onclick = () => {
            delete files[fileName];
            alert(`File "${fileName}" deleted.`);
            updateFileTable();
        };
        deleteCell.appendChild(deleteBtn);
        row.appendChild(deleteCell);

        // Move To input and button
        const moveToCell = document.createElement('td');
        const moveToInput = document.createElement('input');
        moveToInput.type = 'text';
        moveToInput.placeholder = 'Move to...';
        moveToCell.appendChild(moveToInput);
        const moveToBtn = document.createElement('button');
        moveToBtn.textContent = 'Move';
        moveToBtn.className = 'button';
        moveToBtn.onclick = () => {
            const targetFileName = moveToInput.value;
            if (targetFileName) {
                files[targetFileName] = files[fileName];
                files[targetFileName].name = targetFileName;
                delete files[fileName];
                alert(`File "${fileName}" moved to "${targetFileName}".`);
                updateFileTable();
            }
        };
        moveToCell.appendChild(moveToBtn);
        row.appendChild(moveToCell);

        tableBody.appendChild(row);
    }
}

// Initial file table update
updateFileTable();
