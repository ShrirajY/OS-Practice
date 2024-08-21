// File System Data Structure
const fileSystem = {
    '/': {
        'home': {
            'user': {
                'document.txt': 'Hello, world!',
                'image.png': '',
            },
            'other': {}
        },
        'etc': {}
    }
};

// Current Path
let currentPath = '/';

// Function to execute UNIX-like commands
function executeCommand() {
    const command = document.getElementById('commandInput').value.trim();
    const output = document.getElementById('output');
    
    let result;
    switch (command) {
        case 'ls':
            result = listFiles(currentPath);
            break;
        case 'pwd':
            result = currentPath;
            break;
        case 'cd ..':
            result = changeDirectory('..');
            break;
        default:
            if (command.startsWith('cd ')) {
                result = changeDirectory(command.substring(3));
            } else {
                result = `Command not found: ${command}`;
            }
            break;
    }

    output.textContent = result;
    updateFileSystemView();
}

// List files in the current directory
function listFiles(path) {
    const parts = path.split('/').filter(Boolean);
    let current = fileSystem;
    for (const part of parts) {
        if (current[part]) {
            current = current[part];
        } else {
            return 'Directory not found.';
        }
    }
    return Object.keys(current).join('\n') || 'No files found.';
}

// Change the current directory
function changeDirectory(dir) {
    if (dir === '..') {
        const parts = currentPath.split('/').filter(Boolean);
        parts.pop();
        currentPath = '/' + parts.join('/');
    } else if (fileSystem[dir]) {
        currentPath = dir;
    } else {
        return 'Directory not found.';
    }
    return '';
}

// Update the file system visualization
function updateFileSystemView() {
    const fileSystemView = document.getElementById('fileSystem');
    const files = listFiles(currentPath).split('\n');
    fileSystemView.innerHTML = files.map(file => `<div>${file}</div>`).join('');
}

// Initial visualization update
updateFileSystemView();
