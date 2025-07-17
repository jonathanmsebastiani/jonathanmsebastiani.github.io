import numpy as np
import plotly.graph_objects as go
from Bio import PDB

def calculate_per_residue_rmsd(baseline_file, aligned_file):
    parser = PDB.PDBParser(QUIET=True)
    structure1 = parser.get_structure('Baseline', baseline_file)
    structure2 = parser.get_structure('Prediction', aligned_file)

    # Extract C-alpha atoms
    atoms1 = [atom for atom in structure1.get_atoms() if atom.get_name() == 'CA']
    atoms2 = [atom for atom in structure2.get_atoms() if atom.get_name() == 'CA']

    if len(atoms1) != len(atoms2):
        raise ValueError("Mismatch in number of C-alpha atoms between structures")

    # Calculate per-residue RMSD
    rmsd_values = []
    residues = []
    for atom1, atom2 in zip(atoms1, atoms2):
        diff = atom1.coord - atom2.coord
        rmsd = np.sqrt(np.sum(diff**2))
        rmsd_values.append(rmsd)
        residues.append(atom1.get_parent().get_id()[1])  # Residue number

    # Create interactive plot using Plotly
    fig = go.Figure(data=go.Scatter(
        x=residues,
        y=rmsd_values,
        mode='markers+lines',
        marker=dict(color='blue', size=8),
        line=dict(color='blue'),
        hovertemplate='Residue: %{x}<br>RMSD: %{y:.3f} Å<extra></extra>'
    ))

    fig.update_layout(
        title='Per-Residue RMSD Between Baseline and AlphaFold2',
        xaxis_title='Residue Number',
        yaxis_title='RMSD (Å)',
        hovermode='closest',
        template='plotly_white'
    )

    fig.show()

baseline_file = 'AlphaFold2_HBA1_BASELINE_MODEL.pdb'
aligned_file = 'AlphaFold2_HBA1_Protein_Prediction.pdb'

calculate_per_residue_rmsd(baseline_file, aligned_file)
