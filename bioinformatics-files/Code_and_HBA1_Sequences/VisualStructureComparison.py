import nglview as nv
from Bio import PDB

def calculate_and_superimpose(baseline_file, prediction_file):
    parser = PDB.PDBParser(QUIET=True)
    structure1 = parser.get_structure('Baseline', baseline_file)
    structure2 = parser.get_structure('Prediction', prediction_file)
    
    # Extract C-alpha atoms for comparison
    atoms1 = [atom for atom in structure1.get_atoms() if atom.get_name() == 'CA']
    atoms2 = [atom for atom in structure2.get_atoms() if atom.get_name() == 'CA']
    
    if len(atoms1) != len(atoms2):
        raise ValueError("Mismatch in number of C-alpha atoms between structures")

    # Perform superimposition
    super_imposer = PDB.Superimposer()
    super_imposer.set_atoms(atoms1, atoms2)
    super_imposer.apply(structure2.get_atoms())

    # Calculate RMSD
    rmsd_value = super_imposer.rms
    print(f"RMSD after alignment: {rmsd_value:.3f} Å")
    
    # Save aligned structure to a new file
    aligned_file = "aligned_prediction.pdb"
    io = PDB.PDBIO()
    io.set_structure(structure2)
    io.save(aligned_file)
    print(f"Aligned structure saved to {aligned_file}")
    
    return rmsd_value, aligned_file

def visualize_structures(baseline_file, aligned_file):
    view = nv.NGLWidget()

    # Add baseline in blue
    view.add_component(baseline_file)
    view.add_representation('cartoon', color='blue', component=0)

    # Add aligned prediction in red
    view.add_component(aligned_file)
    view.add_representation('cartoon', color='red', component=1)

    view.center()
    return view

baseline_file = 'AlphaFold2_HBA1_BASELINE_MODEL.pdb'
prediction_file = 'AlphaFold2_HBA1_Protein_Prediction.pdb'

# Perform alignment and calculate RMSD
rmsd_value, aligned_file = calculate_and_superimpose(baseline_file, prediction_file)

# Visualize the aligned structures
view = visualize_structures(baseline_file, aligned_file)
view
