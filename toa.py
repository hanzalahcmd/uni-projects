from automata.fa.dfa import DFA
from typing import Dict, Set,List,Tuple
import graphviz
import subprocess
import time 
import copy

def get_user_input(prompt):
    user_input = input(prompt).strip()
    if user_input:
        return user_input.split(",")
    else:
        return []




         

def creteimage(dfa, filename="dfa"):
    dot = graphviz.Digraph()
      # Add states
    for state in dfa.states:
        dot.node(state, shape='circle')
        if state == dfa.initial_state:
            dot.node(state, shape='doublecircle') 
        if state in dfa.final_states:
            dot.node(state, shape='doublecircle') 

    # Add transitions
    for state, transitions in dfa.transitions.items():
        for symbol, next_state in transitions.items():
            dot.edge(state, next_state, label=symbol)

    dot.render(filename, format='png', cleanup=True)
    subprocess.run("start "+filename+".png",shell=True)
  




def statemarker(s_i: str, stateset: Set[str], visited: Dict[str, bool], states: List[str], table: Dict[str, Dict[str, bool]]) -> None:
    stateset.add(s_i)
    if visited[s_i]:
        return
    
    visited[s_i] = True

    for s_j in states:
        if s_j > s_i:
            if not table[s_j][s_i]:
                table[s_j][s_i] = True
                if not visited[s_j]:
                    statemarker(s_j, stateset, visited, states, table)
        elif s_j < s_i:
            if not table[s_i][s_j]:
                table[s_i][s_j] = True
                if not visited[s_j]:
                    statemarker(s_j, stateset, visited, states, table)

def myhill_nerode(allStates: Set[str], transitions: Dict[str, Dict[str, str]], initialState: str, finalStates: Set[str],alph:Set[str]) -> DFA:
    table: Dict[str, Dict[str, bool]] = {}
    
    states: List[str] = sorted(allStates)
    for s_i in states:
        table[s_i] = {}       
        for s_j in states:
            if (s_i in finalStates and s_j in finalStates) or (s_i not in finalStates and s_j not in finalStates):
                table[s_i][s_j] = False
            else:
                table[s_i][s_j] = True

    flag = True
    while flag:
        flag = False
        for s_i in table:
            for s_j in table[s_i]:
                if s_i == s_j:
                    break
                if not table[s_i][s_j]:
                    for sm in alph:
                        a0 = transitions[s_i][sm]
                        b0 = transitions[s_j][sm]
                        if table[a0][b0]:
                            flag = True
                            table[s_i][s_j] = True
                            continue
                    
            

    visited: Dict[str, bool] = {state: False for state in states}
    new_s: Dict[str, Set[str]] = {}
    
    for state in states:
        if not visited[state]:
            stateset = set()
            statemarker(state, stateset, visited, states, table)
            new_s[f"s{len(new_s)}"] = stateset

    new_f_s: Set[str] = set()
    new_t: Dict[str, Dict[str, str]] = {}
    new_ini: str = ""
    
    for key in new_s:
        if initialState in new_s[key]:
            new_ini = key
            break

    for state in new_s:
        oldState = next(iter(new_s[state]))
        if oldState in finalStates:
            new_f_s.add(state)
        t:List[str]=list()
        for s in alph:
            t.append(transitions[oldState][s])
        new_t[state] = {}
        for s_j in new_s:
            for t0 in t:
                for In in alph:
                    if t0 in new_s[s_j]:
                        new_t[state][In] = s_j
    #making a dfa object
    dfastates=list()
    for i in new_s:
        dfastates.append(i)
    print(dfastates)
    dfaacceptstates=list()
    for i in new_f_s:
        dfaacceptstates.append(i)
    print(dfaacceptstates)
    mindfa=DFA(
        states=set(dfastates),
        input_symbols=set(alph),
        transitions=new_t,
        initial_state=new_ini,
        final_states=set(dfaacceptstates)
    )
    return mindfa
    

def get_pno_for_state(partitions: Dict[int, Set[str]], state: str) -> int:
    for key, value_set in partitions.items():
        if state in value_set:
            return key
    return None 


def partition(allStates: Set[str], transitions: Dict[str, Dict[str, str]], initialState: str, finalStates: Set[str],alph:Set[str]) -> DFA:
    partitions:Dict[int,Set[str]]
    partitions={1:set(),2:set()}
    #initial partition
    for i in allStates:
        if(i in finalStates):
            partitions[1].add(i)
        else:
            partitions[2].add(i)
    #partitioning
    changed:bool
    changed=True
    p_copy:Dict[int,Set[str]]
    
    while(changed):
        p_copy=copy.deepcopy(partitions)
        temp_pat:Dict[str,List[int]]
        temp_pat={}
        for i in p_copy:#iterating over partitions
            for j in p_copy[i]:#iterating over states in that partition
                tr_pat:List[int]
                tr_pat=[]
                
                #if(j==first):
                for k in alph:
                        tr_pat.append(get_pno_for_state(p_copy,transitions[j][k]))
                temp_pat[j+f"p{i}"]=tr_pat
        #print("temp_pat is")
        #print(temp_pat)
        stat:Set[str]
        stat=set()
        faltu=set()
        for i in temp_pat.values():
            faltu.add(tuple(i))
        #print(faltu)
        partitions={}
        for f in faltu:
            for i,value in temp_pat.items():
                #print(value)
                #print(f)
                if(tuple(value)==f):
                    
                    #print(i)
                    stat.add(i[:2])
                #print(stat)
            partitions[len(partitions)+1]=stat
            stat=set()
        
        #print(partitions[i])
        if(len(partitions)!=len(p_copy)):
            changed=True
            continue
        for i in partitions.values():
            print(i)
            print(f)
            if(i in p_copy.values()):
                changed=False
            else:
                changed=True
                break    
    newstates=set()    
    for k in partitions:
        newstates.add(f"P{k}")
    n_transitions=dict()
    for ps in newstates:
        n_transitions[ps]={}
    for s in partitions:
        for a in alph:
            num:int
            num=-1
            num=get_pno_for_state(partitions,transitions[next(iter(partitions[s]))][a])
            print(num,s)
            #poo={(f"P{s}",a):f"P{num}"}
            n_transitions[f"P{s}"][a]=f"P{num}"
    #print(n_transitions)    
    
    newstates=sorted(newstates)
    #print(newstates)
    n_final_s=set()
    for f in finalStates:
        num=get_pno_for_state(partitions,f)
        n_final_s.add(f"P{num}")
    #print(n_final_s)
    n_ini=f"P{get_pno_for_state(partitions,initialState)}"
    
    mindfa=DFA(
        states=set(newstates),
        input_symbols=set(alph),
        transitions=n_transitions,
        initial_state=n_ini,
        final_states=set(n_final_s)

    )
    return mindfa
    #print(partitions)
    #print(p_copy)
        


             
                    




def main():
    states = get_user_input("Enter states separated by commas (e.g., q0,q1,q2): ")

    if len(set(states)) != len(states):
        print("Error: States must be unique.")
        return

    alphabet = get_user_input("Enter alphabet symbols separated by commas (e.g., 0,1): ")

    transitions = {}
    for state in states:
        transitions[state] = {}
    print("Enter transitions for each state (current_state, symbol, next_state) separated by commas (e.g., q0,0,q1):")
    all_transitions = get_user_input("")

    for current_state, symbol, next_state in zip(*[all_transitions[::3], all_transitions[1::3], all_transitions[2::3]]):

        current_state = current_state.strip()
        symbol = symbol.strip()
        next_state = next_state.strip()

        if current_state not in states:
            print(f"Invalid state: {current_state}")
            continue

        if symbol not in alphabet:
            print(f"Invalid symbol: {symbol}")
            continue

        if next_state not in states:
            print(f"Invalid state: {next_state}")
            continue

        transitions[current_state][symbol] = next_state

    initial_state = input("Enter initial state: ")
    if initial_state not in states:
        print(f"Invalid state: {initial_state}")
        return

    accepting_states = get_user_input("Enter accepting states separated by commas: ")
    invalid_accepting_states = set(accepting_states) - set(states)
    if invalid_accepting_states:
        print(f"Error: Accepting states must be valid states: {', '.join(invalid_accepting_states)}")
        return
    
    dfa = DFA(
    states=set(states),
    input_symbols=set(alphabet),
    transitions=transitions,
    initial_state=initial_state,
    final_states=set(accepting_states)
    )
    creteimage(dfa,'OG')
    print("now processing the minimization")
    before=time.time()
    minimized_dfa = myhill_nerode(dfa.states,dfa.transitions,dfa.initial_state,dfa.final_states,dfa.input_symbols)
    after=time.time()
    print(f"Time taken to minimize the DFA: {after-before} seconds")
    creteimage(minimized_dfa,'1')
    before1=time.time()
    d=partition(dfa.states,dfa.transitions,dfa.initial_state,dfa.final_states,dfa.input_symbols)
    after1=time.time()
    print(f"Time taken to minimize the DFA: {after1-before1} seconds")
    creteimage(d,'2')
if __name__=="__main__":
 main()





